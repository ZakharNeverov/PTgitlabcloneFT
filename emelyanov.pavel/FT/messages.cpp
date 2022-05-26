#include "messages.h"

#include <iostream>

namespace {
  const std::string EMPTY_ERROR = "<EMPTY>";
  const std::string INVALID_COMMAND_ERROR = "<INVALID COMMAND>";
  const std::string TRUE_MSG = "<TRUE>";
  const std::string FALSE_MSG = "<FALSE>";
}

std::ostream& emelyanov::printEmpty(std::ostream& out)
{
  return out << EMPTY_ERROR;
}

std::ostream& emelyanov::printInvalidCommand(std::ostream& out)
{
  return out << INVALID_COMMAND_ERROR;
}

std::ostream& emelyanov::printTrue(std::ostream& out)
{
  return out << TRUE_MSG;
}

std::ostream& emelyanov::printFalse(std::ostream& out)
{
  return out << FALSE_MSG;
}
