#include "ErrorMessages.h"
#include <iostream>

std::ostream& borisov::printInvalidCommandMessage(std::ostream& out)
{
  out << "<INVALID COMMAND>";
  return out;
}

std::ostream& borisov::printMessageEmpty(std::ostream& out)
{
  out << "<EMPTY>";
  return out;
}

std::ostream& borisov::printInvalidFileFormatMessage(std::ostream& out)
{
  out << "Invalid file format";
  return out;
}
