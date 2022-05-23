#ifndef COMMAND_MESSAGES_HPP
#define COMMAND_MESSAGES_HPP

#include <iosfwd>

namespace maistrenko
{
  void notifyInvalidArguments(std::ostream&);
  void notifyUnexistingDict(std::ostream&);
  void notifyExistingDict(std::ostream&);
  void notifyUnexistingWord(std::ostream&);
  void notifyUnexistingCommand(std::ostream&);
  void notifyUnexistingTranslation(std::ostream&);
}

#endif
