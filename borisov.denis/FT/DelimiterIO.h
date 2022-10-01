#ifndef DELIMITERIO_HPP
#define DELIMITERIO_HPP
#include <iosfwd>

namespace borisov
{
  struct DelimiterIO
  {
    char exp;
  };
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}
#endif
