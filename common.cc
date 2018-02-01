#include "common.hpp"

const char* find_token(uint32_t nb, const char* str) {
  static const char* space = " \t\n";
  for(uint32_t i = 1; i < nb && *str && *str != '\n'; ++i) {
    str += strcspn(str, space);
    str += strspn(str, space);
  }
  return (*str && *str != '\n') ? str : nullptr;
}
