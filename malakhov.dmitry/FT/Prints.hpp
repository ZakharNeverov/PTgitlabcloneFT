#ifndef PRINTS_HPP
#define PRINTS_HPP

#include <ostream>

namespace malakhov
{
  std::ostream& printEmpty(std::ostream& out);
  std::ostream& printTrue(std::ostream& out);
  std::ostream& printFalse(std::ostream& out);
  std::ostream& printInvalid(std::ostream& out);
  std::ostream& printEncoded(std::ostream& out);
  std::ostream& printRaw(std::ostream& out);
}

#endif
