#include <cstdlib>
#include <string>
#include <fstream>

#include <one_cmdline.hpp>

int one_main(int argc, char *argv[]) {
  one_cmdline args(argc, argv);

  std::string line;
  for(auto file : args.file_arg) {
    try {
      std::ifstream is;
      is.exceptions(std::ios::failbit|std::ios::badbit);
      is.open(file);

      int c;

      if(is.peek() == '>') {
        std::getline(is, line);
        std::cout << line << '\n';
      }

      while(true) {
        for(c = is.peek(); c != '>' && c != EOF; c = is.peek()) {
          std::getline(is, line);
          std::cout << line;
        }
        std::cout << '\n';
        if(c == EOF) break;
        std::getline(is, line);
        std::cout << line << '\n';
      }
    } catch(std::ios::failure) {
      std::cerr << "Error with file '" << file << '\'' << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
