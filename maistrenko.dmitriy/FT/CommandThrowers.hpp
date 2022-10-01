#ifndef COMMAND_THROWERS_HPP
#define COMMAND_THROWERS_HPP

namespace maistrenko
{
  void raiseInvalidArguments();
  void raiseUnexistingDict();
  void raiseExistingDict();
  void raiseUnexistingWord();
  void raiseUnexistingCommand();
  void raiseUnexistingTranslation();
  void raiseUnexistingFile();
  void raiseInvalidFile();
}

#endif
