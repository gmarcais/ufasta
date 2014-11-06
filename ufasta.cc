#include <cstring>
#include <iostream>

typedef int (main_func_t)(int argc, char *argv[]);

main_func_t hgrep_main;
main_func_t one_main;
main_func_t sizes_main;
main_func_t head_main;

main_func_t sos;
main_func_t version;

struct cmd_func {
  const char  *cmd;
  main_func_t *func;
};
cmd_func cmd_list[] = {
  {"hgrep",             &hgrep_main},
  {"one",               &one_main},
  {"sizes",             &sizes_main},
  {"head",              &head_main},

  /* help in all its form. Must be first non-command */
  {"help",              &sos},
  {"-h",                &sos},
  {"-help",             &sos},
  {"--help",            &sos},
  {"-?",                &sos},
  {"--version",         &version},
  {"-V",                &version},
  {"",                  0}
};



void __sos(std::ostream *os)
{
  *os << "Usage: ufasta <cmd> [options] arg..."  << std::endl <<
    "Where <cmd> is one of: ";
  bool comma = false;
  for(cmd_func *ccmd = cmd_list; ccmd->func != sos; ccmd++) {
    *os << (comma ? ", " : "") << ccmd->cmd;
    comma = true;
  }
  *os << "." << std::endl;
  *os << "Options:" << std::endl <<
    "  --version        Display version" << std::endl <<
    "  --help           Display this message" << std::endl;
}

int sos(int argc, char *argv[])
{
  __sos(&std::cout);
  return 0;
}

int version(int argc, char *argv[])
{
#ifdef PACKAGE_STRING
  std::cout << PACKAGE_STRING << std::endl;
#else
  std::cout << "0.0.0" << std::endl;
#endif
  return 0;
}

int main(int argc, char *argv[])
{
  std::string error;

  if(argc < 2) {
    error = "Too few arguments";
  } else {
    for(cmd_func *ccmd = cmd_list; ccmd->func != 0; ccmd++) {
      if(!strcmp(ccmd->cmd, argv[1]))
        return ccmd->func(argc - 1, argv + 1);
    }
    error = "Unknown command '";
    error += argv[1];
    error += "'\n";
  }

  std::cerr << error << std::endl;
  __sos(&std::cerr);
  return 1;
}
