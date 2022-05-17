#include "printMessages.h"
#include <iostream>

namespace dudnik
{
  std::ostream& printEmpty(std::ostream& out)
  {
    out << "<EMPTY>" << std::endl;
    return out;
  }
  std::ostream& printNoFileToRead(std::ostream& out)
  {
    out << "No file to read" << std::endl;
    return out;
  }
  std::ostream& printUnableToReadFile(std::ostream& out)
  {
    out << "Unable to read the file" << std::endl;
    return out;
  }
  std::ostream& printInvalidCommand(std::ostream& out)
  {
    out << "<INVALID COMMAND>" << std::endl;
    return out;
  }
  std::ostream& printBadNumberOfArgs(std::ostream& out)
  {
    out << "Bad number of args" << std::endl;
    return out;
  }
  std::ostream& printUnableToSaveResults(std::ostream& out)
  {
    out << "Unable to open the file in order to save results" << std::endl;
    return out;
  }
}
