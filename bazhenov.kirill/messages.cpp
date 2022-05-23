#include "messages.hpp"
#include <iostream>

std::ostream& bazhenov::printInvalidCommand(std::ostream& out)
{
  return out << "<INVALID COMMAND>";
}

std::ostream& bazhenov::printEmpty(std::ostream& out)
{
  return out << "<EMPTY>";
}

std::ostream& bazhenov::printNotExist(std::ostream& out)
{
  return out << "The dictionary with this name doesn't exist!";
}

std::ostream& bazhenov::printNoMatches(std::ostream& out)
{
  return out << "No matches found!";
}
