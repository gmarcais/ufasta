#include <cstdlib>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>

#include <common.hpp>
#include <tail_cmdline.hpp>

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

static int tail_positive(const tail_cmdline& args, int entries) {
  int                res = EXIT_SUCCESS;
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
        ce.size = 0;
        if(c == '>') {
          ce.add_line(is);
          c = is.peek();
        }
        for( ; c != '>' && c != EOF; c = is.peek())
          ce.add_line(is);
      }

      // Output cache
      for(int i = 0; i < cz; ++i) {
        const entry& ce = cache[i];
        for(int j = 0; j < ce.size; ++j)
          std::cout << ce.lines[j] << '\n';
      }
    } catch(std::ios::failure) {
      std::cerr << "Error with file '" << file << '\'' << std::endl;
      res = EXIT_FAILURE;
    }
  }

  return res;
}

static int tail_negative(const tail_cmdline& args, const int entries) {
  int res = EXIT_SUCCESS;

  for(const auto& file : args.file_arg) {
    try {
      std::ifstream is;
      is.exceptions(std::ios::failbit|std::ios::badbit);
      is.open(file);
      if((args.file_arg.size() > 1 && !args.quiet_flag) || args.verbose_flag)
        std::cout << "==> " << file << " <==\n";
      int c = is.peek();
      for(int i = 1; c != EOF && i < entries; ++i) {
        if(c == '>') {
          skip_line(is);
          c = is.peek();
        }
        for( ; c != '>' && c != EOF; c = is.peek())
          skip_line(is);
      }
      if(c != EOF)
        std::cout << is.rdbuf();
    } catch(std::ios::failure) {
      std::cerr << "Error with file '" << file << '\'' << std::endl;
      res = EXIT_FAILURE;
    }
  }

  return res;
}

static int parse_nb_entries(const std::string& s) {
  if(s.empty()) return 0;
  if(s[0] == '+') return -std::stoi(s.substr(1));
  return std::stoi(s);
}

int tail_main(int argc, char *argv[]) {
  const tail_cmdline args(argc, argv);

  int entries;
  try {
    entries = parse_nb_entries(args.entries_arg);
  } catch(std::invalid_argument) {
    std::cerr << "Invalid number of entries '" << args.entries_arg << '\'' << std::endl;
    return EXIT_FAILURE;
  }

  if(entries == 0) return EXIT_SUCCESS;
  if(entries > 0)
    return tail_positive(args, entries);
  else
    return tail_negative(args, abs(entries));

  return EXIT_SUCCESS;
}
