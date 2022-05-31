#ifndef DELIMETER_HPP
#define DELIMETER_HPP
#include <iosfwd>

namespace nefedev
{
  struct DelimeterIO
  {
    char exp;
  };
  std::istream& operator>>(std::istream& in, nefedev::DelimeterIO&& dest);
}

#endif
