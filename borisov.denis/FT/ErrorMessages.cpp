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

std::ostream& borisov::printInvalidFileFormat(std::ostream& out)
{
  out << "Invalid file format";
  return out;
}

std::ostream& borisov::printInvalidNumberOfArguments(std::ostream& out)
{
  out << "Invalid number of arguments";
  return out;
}

std::ostream& borisov::printFailToOpenFile(std::ostream& out)
{
  out << "Fail to open file";
  return out;
}
