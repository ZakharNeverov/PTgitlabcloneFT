#include "printInformation.hpp"
namespace matyushenko {
  std::ostream &printEmpty(std::ostream &out)
  {
    out << "<EMPTY>";
    return out;
  }
  std::ostream &printNoFileToRead(std::ostream &out)
  {
    out << "No file to read";
    return out;
  }
  std::ostream &printUnableToReadFile(std::ostream &out)
  {
    out << "Unable to read the file";
    return out;
  }
  std::ostream &printInvalidCommand(std::ostream &out)
  {
    out << "<INVALID COMMAND>";
    return out;
  }
  std::ostream &printBadNumberOfArgs(std::ostream &out)
  {
    out << "Bad number of args";
    return out;
  }
  std::ostream &printTrue(std::ostream &out)
  {
    out << "<TRUE>";
    return out;
  }
  std::ostream &printFalse(std::ostream &out)
  {
    out << "<FALSE>";
    return out;
  }
}
