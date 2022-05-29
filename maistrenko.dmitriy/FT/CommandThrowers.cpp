#include "CommandThrowers.hpp"
#include <stdexcept>

void maistrenko::raiseInvalidArguments()
{
  throw std::invalid_argument("<INVALID ARGUMENTS>\n");
}

void maistrenko::raiseUnexistingDict()
{
  throw std::invalid_argument("<DICTIONARY DOES NOT EXIST>\n");
}

void maistrenko::raiseExistingDict()
{
  throw std::invalid_argument("<DICTIONARY DOES ALREADY EXIST>\n");
}

void maistrenko::raiseUnexistingWord()
{
  throw std::invalid_argument("<WORD DOES NOT EXIST IN THIS DICTIONARY>\n");
}

void maistrenko::raiseUnexistingCommand()
{
  throw std::invalid_argument("<COMMAND DOES NOT EXIST>\n");
}

void maistrenko::raiseUnexistingTranslation()
{
  throw std::invalid_argument("<WORD DOES NOT HAVE SUCH TRANSLATION>\n");
}

void maistrenko::raiseUnexistingFile()
{
  throw std::invalid_argument("<FILE-SOURCE DOES NOT EXIST>\n");
}

void maistrenko::raiseInvalidFile()
{
  throw std::invalid_argument("<DESTINATION FILE IS INVALID>\n");
}
