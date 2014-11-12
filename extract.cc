#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
#include <unordered_set>

#include "common.hpp"
#include <extract_cmdline.hpp>


std::unordered_set<std::string> read_names(const extract_cmdline& args) {
  std::unordered_set<std::string> res;

  for(const auto& n : args.name_arg)
    res.insert(n);
  std::string entry;
  for(const auto& file : args.name_file_arg) {
    std::ifstream is(file);
    if(is.fail())
      extract_cmdline::error() << "Error opening file '" << file << "' " << is.bad();
    for(is >> entry; is.good(); is >> entry)
      res.insert(entry);
  }
  return res;
}

int extract_main(int argc, char *argv[]) {
  extract_cmdline args(argc, argv);

  const auto entries = read_names(args);
  const auto end     = entries.cend();

  size_t max_count = args.max_count_given ? args.max_count_arg : std::numeric_limits<size_t>::max();
  std::string line;
  for(auto file : args.file_arg) {
    try {
      std::ifstream is;
      is.exceptions(std::ios::failbit|std::ios::badbit);
      is.open(file);
      bool          skip = true;
      int           c;
      // Display unchanged up to first header
      for(c = is.peek(); c != '>' && c != EOF; c = is.peek()) {
        std::getline(is, line);
        std::cout << line << '\n';
      }

      while(c != EOF) {
        std::getline(is, line);
        const auto space = line.find_first_of(" \t", 1);
        skip = (entries.find(line.substr(1, space - 1)) == end) ^ args.invert_match_flag;
        if(skip) {
          for(c = is.peek(); c != '>' && c != EOF; c = is.peek())
            skip_line(is);
        } else {
          if(max_count-- == 0) break;
          std::cout << line << '\n';
          for(c = is.peek(); c != '>' && c != EOF; c = is.peek()) {
            std::getline(is, line);
            std::cout << line << '\n';
          }
        }
      }
    } catch(std::ios::failure) {
      std::cerr << "Error with file '" << file << '\'' << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
