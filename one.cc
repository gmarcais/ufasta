#include <cstdlib>
#include <string>
#include <fstream>

#include <one_cmdline.hpp>

int one_main(int argc, char *argv[]) {
  one_cmdline args(argc, argv);

  std::string line;
  for(auto file : args.file_arg) {
    std::ifstream is(file);
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
  }

  return EXIT_SUCCESS;
}
