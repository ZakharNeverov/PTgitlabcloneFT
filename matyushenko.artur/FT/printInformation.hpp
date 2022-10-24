#ifndef PRINTINFORMATION_HPP
#define PRINTINFORMATION_HPP
#include <ostream>
namespace matyushenko {
  std::ostream &printEmpty(std::ostream &out);
  std::ostream &printTrue(std::ostream &out);
  std::ostream &printFalse(std::ostream &out);
  std::ostream &printNoFileToRead(std::ostream &out);
  std::ostream &printUnableToReadFile(std::ostream &out);
  std::ostream &printInvalidCommand(std::ostream &out);
  std::ostream &printBadNumberOfArgs(std::ostream &out);
}
#endif
