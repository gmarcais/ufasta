#ifndef __COMMON_H__
#define __COMMON_H__

#include <istream>
#include <limits>

inline std::istream& skip_line(std::istream& is) {
  return is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#endif /* __COMMON_H__ */
