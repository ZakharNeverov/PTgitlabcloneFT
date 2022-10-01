#ifndef FORREADING_HPP
#define FORREADING_HPP

#include <iosfwd>

namespace shkroba
{
  struct DelimiterIO
  {
    char exp;
  };
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}

#endif
