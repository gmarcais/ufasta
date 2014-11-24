#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <type_traits>
#include <string>
#include <algorithm>

#include <sort_cmdline.hpp>

static sort_cmdline args;

struct close_fd {
  int         fd;
  char*       ptr;
  off_t       size;

  close_fd() : fd(-1), ptr(nullptr), size(0) { }
  close_fd(int i) : fd(i), ptr(nullptr), size(0) { }
  close_fd(const close_fd&) = delete;
  close_fd(close_fd&& rhs) noexcept : fd(rhs.fd), ptr(rhs.ptr), size(rhs.size) {
    rhs.fd   = -1;
    rhs.ptr  = nullptr;
    rhs.size = 0;
  }
  ~close_fd() {
    close();
    unmap();
  }
  void close() {
    if(fd >= 0) ::close(fd);
    fd = -1;
  }
  void unmap() {
    if(ptr != nullptr) munmap((void*)ptr, size);
  }
};

template<typename T>
struct header_type {
  T           value;
  const char* start;
  const char* end;
  header_type(T v, const char* s, const char* e) : value(v), start(s), end(e) { }
};

void slurp_in(close_fd& fd) {
  fd.size = 1024 * 1024;
  fd.ptr  = (char*)mmap(0, fd.size, PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if(fd.ptr == MAP_FAILED) return;

  size_t offset = 0;
  while(true) {
    size_t left = fd.size - offset - 1;
    while(left > 0) {
      ssize_t res = read(fd.fd, fd.ptr + offset, left);
      if(res == -1 && errno == EINTR) continue;
      if(res == -1) goto error;
      if(res == 0) {
        size_t new_size = offset + 1;
        void*  new_ptr  = mremap(fd.ptr, fd.size, new_size, MREMAP_MAYMOVE);
        if(new_ptr == MAP_FAILED) goto error;
        fd.size = new_size;
        fd.ptr = (char*)new_ptr;
        return;
      }
      offset += res;
      left   -= res;
    }
    size_t new_size = fd.size * 2;
    void*  new_ptr  = mremap(fd.ptr, fd.size, new_size, MREMAP_MAYMOVE);
    if(new_ptr == MAP_FAILED) goto error;
    fd.size = new_size;
    fd.ptr  = (char*)new_ptr;
  }

 error:
  int save_errno = errno;
  fd.unmap();
  errno = save_errno;
}

close_fd open_mmap(const char* path) {
  close_fd res(open(path, O_RDONLY));
  if(res.fd == -1)
    sort_cmdline::error() << "Failed to open file '" << path << "':" << strerror(errno);
  struct stat file_stat;
  memset(&file_stat, 0, sizeof(file_stat));
  if(fstat(res.fd, &file_stat) == -1)
    sort_cmdline::error() << "Failed to stat file '" << path << "':" << strerror(errno);
  res.size = file_stat.st_size + 1;
  res.ptr = (char*)mmap(nullptr, res.size, PROT_READ, MAP_PRIVATE, res.fd, 0);
  if(res.ptr == MAP_FAILED) {
    slurp_in(res);
    if(res.ptr == MAP_FAILED)
      sort_cmdline::error() << "Failed to read file '" << path << "':" << strerror(errno);
  }
  res.close();
  return res;
}

template<typename T>
struct header_traits { };

// Key is an signed int. Sort numerically
template<>
struct header_traits<int64_t> {
  typedef header_type<int64_t> type;

  static type name(const char* start, const char* end) {
    return type(std::stoll(std::min(start + args.character_arg, end)), start, end);
  }

  static void sort(std::vector<type>& headers) {
    std::sort(headers.begin(), headers.end(), [](const type& x, const type& y) { return x.value < y.value; });
  }
};

// Key is a C string (type size_t is its length). Sort alphabetically.
struct str_type {
  size_t size;
};
template<>
struct header_traits<str_type> {
  typedef header_type<str_type> type;

  static type name(const char* start, const char* end) {
    str_type s = { strcspn(std::min(start + args.character_arg, end), " \n\t") };
    return type(s, start, end);
  }

  static void sort(std::vector<type>& headers) {
    std::sort(headers.begin(), headers.end(),
              [](const type& x, const type& y) -> bool {
                int res = memcmp(x.start + 1, y.start + 1, std::min(x.value.size, y.value.size));
                return res != 0 ? res < 0 : x.value.size < y.value.size;
              });
  }
};

// Key is a C string, sort alphabetically case insensitive.
struct ustr_type {
  size_t size;
};
template<>
struct header_traits<ustr_type> {
  typedef header_type<ustr_type> type;

  static type name(const char* start, const char* end) {
    ustr_type s = { strcspn(std::min(start + args.character_arg, end), " \n\t") };
    return type(s, start, end);
  }

  static void sort(std::vector<type>& headers) {
    std::sort(headers.begin(), headers.end(),
              [](const type& x, const type& y) -> bool {
                int res = strncasecmp(x.start + 1, y.start + 1, std::min(x.value.size, y.value.size));
                return res != 0 ? res < 0 : x.value.size < y.value.size;
              });
  }
};

// No key. Sort randomly
struct random_type { };
template<>
struct header_traits<random_type> {
  typedef header_type<random_type> type;

  static type name(const char* start, const char* end) {
    return type(random_type(), start, end);
  }

  static void sort(std::vector<type>& headers) {
    std::random_shuffle(headers.begin(), headers.end());
  }
};

template<typename T>
void parse_headers(const char* const start, const char* const end, std::vector<header_type<T>>& headers) {
  const char* current = start;
  // Ignore up to first header
  if(*current != '>') {
    for(current = strchr(current, '>'); current; current = strchr(current + 1, '>'))
      if(*(current - 1) == '\n') break;
  }

  // At the beginning of the loop, current points to the start of the
  // header '>'
  for(const char* next = current + 1; current; current = next) {
    for(next = strchr(next + 1, '>'); next; next = strchr(next + 1, '>'))
      if(*(next - 1) == '\n') break;
    headers.push_back(header_traits<T>::name(current, next ? next : end - 1));
  }
}

template <typename T>
static int sort_mmap(const sort_cmdline& args) {
  static_assert(std::is_nothrow_move_constructible<close_fd>::value, "Close_fd nothrow move");
  std::vector<close_fd>       fds;
  std::vector<header_type<T>> headers;

  for(auto path : args.file_arg) {
    fds.push_back(open_mmap(path));
    auto& current = fds.back();
    parse_headers<T>(current.ptr, current.ptr + current.size, headers);
  }

  header_traits<T>::sort(headers);

  for(const auto& header : headers)
    std::cout.write(header.start, header.end - header.start);

  return EXIT_SUCCESS;
}

int sort_main(int argc, char *argv[]) {
  args.parse(argc, argv);

  if(args.numeric_sort_flag)
    return sort_mmap<int64_t>(args);
  else if(args.random_sort_flag)
    return sort_mmap<random_type>(args);
  else if(args.ignore_case_flag)
    return sort_mmap<ustr_type>(args);
  else
    return sort_mmap<str_type>(args);
}
