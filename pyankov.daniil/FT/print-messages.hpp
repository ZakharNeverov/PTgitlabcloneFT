#ifndef PRINT_MESSAGES_HPP
#define PRINT_MESSAGES_HPP
#include <iosfwd>
namespace pyankov
{
  std::ostream& printInvalidCommand(std::ostream& out);
  std::ostream& printTrue(std::ostream& out);
  std::ostream& printFalse(std::ostream& out);
}
#endif
