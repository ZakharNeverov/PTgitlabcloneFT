#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "iosfwd"

namespace bazhenov {
  std::ostream& printInvalidCommand(std::ostream& out);
  std::ostream& printEmpty(std::ostream& out);
  std::ostream& printNotExist(std::ostream& out);
}

#endif
