#ifndef DELIMITER_IO_HPP
#define DELIMITER_IO_HPP

#include <iosfwd>

namespace gusarov {
  struct DelimiterIO
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}
#endif
