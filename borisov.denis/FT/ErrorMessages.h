#ifndef ERROR_MESSAGES_H
#define ERROR_MESSAGES_H
#include <iosfwd>

namespace borisov
{
  std::ostream& printInvalidCommandMessage(std::ostream& out);
  std::ostream& printMessageEmpty(std::ostream& out);
  std::ostream& printInvalidFileFormat(std::ostream& out);
  std::ostream& printInvalidNumberOfArguments(std::ostream& out);
  std::ostream& printFailToOpenFile(std::ostream& out);
}
#endif
