#ifndef MESSAGESIO_H
#define MESSAGESIO_H
#include <iosfwd>

namespace khudyakov
{
  std::ostream& invalidCommandMessage(std::ostream&);
  std::ostream& emptyMessage(std::ostream&);
  std::ostream& errorOpenFileMessage(std::ostream&);
  std::ostream& invalidArgumentMessage(std::ostream&);
}

#endif
