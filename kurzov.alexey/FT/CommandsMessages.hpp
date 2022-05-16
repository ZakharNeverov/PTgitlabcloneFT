#ifndef COMMANDS_MESSAGES_HPP
#define COMMANDS_MESSAGES_HPP

#include <iosfwd>

namespace kurzov
{
  void outError(std::ostream&);
  void outEmpty(std::ostream&);
  void outTrue(std::ostream&);
  void outFalse(std::ostream&);
  void outNone(std::ostream&);
}

#endif
