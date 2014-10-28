#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
#include <boost/regex.hpp>

#include <hgrep_cmdline.hpp>


int hgrep_main(int argc, char *argv[]) {
  hgrep_cmdline args(argc, argv);
  boost::regex regexp;

  try {
    regexp = args.regexp_arg;
  } catch(boost::bad_expression error) {
    std::cerr << "Invalid regular expression: " << error.what() << "\n"
              << "  " << args.regexp_arg << "\n"
              << std::string(1 + error.position(), ' ') << "^" << std::endl;
    return EXIT_FAILURE;
  }

  std::string line;
  for(auto file : args.file_arg) {
    std::ifstream is(file);
    bool          skip = true;
    int           c;
    while(true) {
      if(skip) {
        for(c = is.peek(); c != '>' && c != EOF; c = is.peek())
          is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      } else {
        for(c = is.peek(); c != '>' && c != EOF; c = is.peek()) {
          std::getline(is, line);
          std::cout << line << '\n';
        }
      }
      if(c == EOF) break;
      std::getline(is, line);
      skip = !regex_search(++line.cbegin(), line.cend(), regexp);
      if(!skip)
        std::cout << line << '\n';
    }
  }

  return EXIT_SUCCESS;
}
