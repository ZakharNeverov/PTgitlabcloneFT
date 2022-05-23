#include "messages.h"

#include <iostream>

namespace {
  const std::string EMPTY_ERROR = "<EMPTY>";
  const std::string INVALID_COMMAND_ERROR = "<INVALID COMMAND>";
}

std::ostream& emelyanov::printEmpty(std::ostream& out)
{
  return out << EMPTY_ERROR;
}

std::ostream& emelyanov::printInvalidCommand(std::ostream& out)
{
  return out << INVALID_COMMAND_ERROR;
}
