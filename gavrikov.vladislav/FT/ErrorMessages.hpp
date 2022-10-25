#ifndef ERROR_MESSAGES_HPP
#define ERROR_MESSAGES_HPP

#include <ostream>

namespace gavrikov
{
  std::ostream& errorNameMessage(std::ostream& out);
  std::ostream& emptyCollectionMessage(std::ostream& out);
  std::ostream& nameRedefinitionMessage(std::ostream& out);
  std::ostream& errorOpenFile(std::ostream& out);
}
#endif
