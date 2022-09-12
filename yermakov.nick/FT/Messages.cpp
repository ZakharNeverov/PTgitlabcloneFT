#include "Messages.hpp"

#include <ostream>

std::ostream& yermakov::printInvalid(std::ostream& out)
{
  return out << "<INVALID COMMAND>";
}
