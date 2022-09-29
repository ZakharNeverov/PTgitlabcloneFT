#include "Prints.hpp"

namespace
{
  const char* const EMPTY_MSG = "<EMPTY>";
  const char* const TRUE_MSG = "<TRUE>";
  const char* const FALSE_MSG = "<FALSE>";
  const char* const INVALID_MSG = "<INVALID COMMAND>";
  const char* const ENCODED_MSG = "<ENCODED>";
  const char* const RAW_MSG = "<RAW>";
}

std::ostream& malakhov::printEmpty(std::ostream& out)
{
  return out << EMPTY_MSG;
}

std::ostream& malakhov::printTrue(std::ostream& out)
{
  return out << TRUE_MSG;
}

std::ostream& malakhov::printFalse(std::ostream& out)
{
  return out << FALSE_MSG;
}

std::ostream& malakhov::printInvalid(std::ostream& out)
{
  return out << INVALID_MSG;
}

std::ostream& malakhov::printEncoded(std::ostream& out)
{
  return out << ENCODED_MSG;
}

std::ostream& malakhov::printRaw(std::ostream& out)
{
  return out << RAW_MSG;
}
