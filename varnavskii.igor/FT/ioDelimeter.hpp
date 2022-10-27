#ifndef IODELIMETER_HPP
#define IODELIMETER_HPP

#include <iosfwd>

namespace varnavskii
{
  struct DelimeterIO
  {
    char exp;
  };

  std::istream &operator>>(std::istream &, DelimeterIO &&);
}

#endif
