#ifndef IO_OPERATIONS_H
#define IO_OPERATIONS_H
#include <string>
#include <iosfwd>

namespace kolosovskaya
{
  std::string getPartOfStr(std::string&, char);
  std::ostream& printEmptyDictMessage(std::ostream&);
  std::ostream& printWrongCommandMessage(std::ostream&);
  std::ostream& printNotExistedDictMessage(std::ostream&);
  std::ostream& printExistedDictMessage(std::ostream&);
  std::ostream& printWrongArgsMessage(std::ostream&);
}

#endif 

