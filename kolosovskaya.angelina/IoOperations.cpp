#include "IoOperations.h"
#include <string>
#include <iostream>

namespace kolosovskaya
{
  std::string getPartOfStr(std::string& str, char del)
  {
    size_t space = str.find_first_of(del, 0);
    if (space == 0)
    {
      str.erase(0, 1);
      space = str.find_first_of(del, 0);
      std::string part = str.substr(0, space);
      size_t delim = (space == std::string::npos) ? space : space + 1;
      str.erase(0, delim);
      return part;
    }
    std::string part = str.substr(0, space);
    size_t delim = (space == std::string::npos) ? space : space + 1;
    str.erase(0, delim);
    return part;
  }

  std::ostream& printEmptyDictMessage(std::ostream& out)
  {
    return out << "This dict is empty!\n";
  }

  std::ostream& printWrongCommandMessage(std::ostream& out)
  {
    return out << "Wrong command!\n";
  }

  std::ostream& printNotExistedDictMessage(std::ostream& out)
  {
    return out << "The dict isn't existed yet!\n";
  }

  std::ostream& printExistedDictMessage(std::ostream& out)
  {
    return out << "The dict is existed yet!\n";
  }

  std::ostream& printWrongArgsMessage(std::ostream& out)
  {
    return out << "Wrong args!\n";
  }
}
