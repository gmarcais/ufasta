#include <cstdlib>
#include <string>
#include <fstream>

#include "common.hpp"
#include <sizes_cmdline.hpp>

int sizes_main(int argc, char *argv[]) {
  sizes_cmdline args(argc, argv);

  std::string line;
  for(auto file : args.file_arg) {
    try {
      std::ifstream is;
      is.exceptions(std::ios::failbit|std::ios::badbit);
      is.open(file);

      int c;

      // Skip up to first header
      for(c = is.peek(); c != '>' && c != EOF; c = is.peek())
        skip_line(is);

      while(c != EOF) {
        if(!args.header_flag) {
          skip_line(is);
        } else {
          std::getline(is, line);
          auto last = line.find_first_of(" \t");
          std::cout.write(line.c_str() + 1, last - 1);
          std::cout << ' ';
          //          std::cout << line.substr(1, last - 1) << ' ';
        }

        size_t size = 0;
        for(c = is.peek(); c != '>' && c != EOF; c = is.peek()) {
          std::getline(is, line);
          size += line.size();
        }
        std::cout << size << '\n';
      }
    } catch(std::ios::failure) {
      std::cerr << "Error with file '" << file << '\'' << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
