#include <cstdlib>
#include <string>
#include <fstream>

#include <head_cmdline.hpp>

int head_negative(const head_cmdline& args) {
  std::cerr << "Not implemented" << std::endl;
  return EXIT_FAILURE;
}

int head_positive(const head_cmdline& args) {
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
