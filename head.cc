#include <cstdlib>
#include <string>
#include <fstream>

#include <head_cmdline.hpp>

struct entry {
  int                      size;
  std::vector<std::string> lines;
  void add_line(std::istream& is) {
    if(size >= (int)lines.size())
      lines.push_back("");
    std::getline(is, lines[size]);
    ++size;
  }
};

static int head_negative(const head_cmdline& args) {
  int                res     = EXIT_SUCCESS;
  const int          entries = abs(args.entries_arg);
  std::vector<entry> cache(entries);

  for(const auto& file : args.file_arg) {
    try {
      std::ifstream is;
      is.exceptions(std::ios::failbit|std::ios::badbit);
      is.open(file);
      if((args.file_arg.size() > 1 && !args.quiet_flag) || args.verbose_flag)
        std::cout << "==> " << file << " <==\n";

      int cz = 0; // cache size
      // Read into cache
      int c  = is.peek();
      for(int i = 0; c != EOF; cz = std::min(cz + 1, entries), i = (i + 1) % entries) {
        entry& ce = cache[i];
        if(cz == entries) { // Print with a delay
          for(int j = 0; j < ce.size; ++j)
            std::cout << ce.lines[j] << '\n';
        }
        ce.size = 0;
        if(c == '>') {
          ce.add_line(is);
          c = is.peek();
        }
        for( ; c != '>' && c != EOF; c = is.peek())
          ce.add_line(is);
      }

    } catch(std::ios::failure) {
      std::cerr << "Error with file '" << file << '\'' << std::endl;
      res = EXIT_FAILURE;
    }
  }

  return res;
}

static int head_positive(const head_cmdline& args) {
  int res = EXIT_SUCCESS;

  std::string line;
  for(const auto& file : args.file_arg) {
    try {
      std::ifstream is;
      is.exceptions(std::ios::failbit|std::ios::badbit);
      is.open(file);
      if((args.file_arg.size() > 1 && !args.quiet_flag) || args.verbose_flag)
        std::cout << "==> " << file << " <==\n";
      int c = is.peek();
      for(int i = 0; c != EOF && i < args.entries_arg; ++i) {
        if(c == '>') {
          std::getline(is, line);
          std::cout << line << '\n';
          c = is.peek();
        }
        for( ; c != '>' && c != EOF; c = is.peek()) {
          std::getline(is, line);
          std::cout << line << '\n';
        }
      }
    } catch(std::ios::failure) {
      std::cerr << "Error with file '" << file << '\'' << std::endl;
      res = EXIT_FAILURE;
    }
  }

  return res;
}

int head_main(int argc, char *argv[]) {
  const head_cmdline args(argc, argv);

  if(args.entries_arg == 0) return EXIT_SUCCESS;
  if(args.entries_arg > 0)
    return head_positive(args);
  else
    return head_negative(args);
}
