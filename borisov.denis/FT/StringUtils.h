#ifndef STRING_UTILS_H
#define STRING_UTILS_H
#include <string>
#include <list>

namespace borisov
{
  namespace utils
  {
    void split(const std::string& s, std::list< std::string >& list, char separator = ' ');
  }
}
#endif
