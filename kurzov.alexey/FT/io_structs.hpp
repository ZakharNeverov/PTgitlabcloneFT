#ifndef IO_STRUCTS_HPP
#define IO_STRUCTS_HPP

#include <iosfwd>

namespace kurzov
{
  struct DelimeterIO
  {
    char exp;
  };
  std::istream& operator>>(std::istream&, DelimeterIO&&);
}

#endif
