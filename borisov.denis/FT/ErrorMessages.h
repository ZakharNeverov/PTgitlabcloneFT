#ifndef ERROR_MESSAGES_H
#define ERROR_MESSAGES_H
#include <iosfwd>

namespace borisov
{
  std::ostream& printInvalidCommandMessage(std::ostream& out);
  std::ostream& printMessageEmpty(std::ostream& out);
  std::ostream& printInvalidFileFormatMessage(std::ostream& out);
}
#endif
