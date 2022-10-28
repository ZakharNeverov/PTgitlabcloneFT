#include "messages.hpp"

#include <iostream>

std::ostream& bazzhin::printInvalidCommand(std::ostream& out)
{
  return out << "<INVALID COMMAND>";
}

std::ostream& bazzhin::printEmpty(std::ostream& out)
{
  return out << "<EMPTY>";
}

std::ostream& bazzhin::printInvalidArguments(std::ostream& out)
{
  return out << "<INVALID ARGUMENTS>";
}

std::ostream& bazzhin::printAlreadyExists(std::ostream& out)
{
  return out << "<DICTIONARY ALREADY EXISTS>";
}

std::ostream& bazzhin::printNotExists(std::ostream& out)
{
  return out << "<DICTIONARY DOES NOT EXIST>";
}

std::ostream& bazzhin::printNotEnoughArguments(std::ostream& out)
{
  return out << "<NOT ENOUGH ARGUMENTS>";
}

std::ostream& bazzhin::printMissingWord(std::ostream& out)
{
  return out << "< WORD IS MISSING>";
}
