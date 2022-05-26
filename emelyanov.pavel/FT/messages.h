#ifndef MESSAGES_H
#define MESSAGES_H

#include <iosfwd>

namespace emelyanov {
  std::ostream& printEmpty(std::ostream& out);
  std::ostream& printInvalidCommand(std::ostream& out);
  std::ostream& printTrue(std::ostream& out);
  std::ostream& printFalse(std::ostream& out);
}

#endif
