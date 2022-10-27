#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "iosfwd"

namespace bazzhin
{
  std::ostream& printInvalidCommand(std::ostream& out);
  std::ostream& printInvalidArguments(std::ostream& out);
  std::ostream& printEmpty(std::ostream& out);
  std::ostream& printAlreadyExists(std::ostream& out);
  std::ostream& printNotExists(std::ostream& out);
  std::ostream& printNotEnoughArguments(std::ostream& out);
  std::ostream& printMissingWord(std::ostream& out);
}

#endif
