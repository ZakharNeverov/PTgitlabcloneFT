#include "CommandMessages.hpp"
#include <ostream>

void maistrenko::notifyInvalidArguments(std::ostream& out)
{
  out << "<INVALID ARGUMENTS>\n";
}

void maistrenko::notifyUnexistingDict(std::ostream& out)
{
  out << "<DICTIONARY DOES NOT EXIST>\n";
}

void maistrenko::notifyExistingDict(std::ostream& out)
{
  out << "<DICTIONARY DOES ALREADY EXIST>\n";
}

void maistrenko::notifyUnexistingWord(std::ostream& out)
{
  out << "<WORD DOES NOT EXIST IN THIS DICTIONARY>\n";
}

void maistrenko::notifyUnexistingCommand(std::ostream& out)
{
  out << "<COMMAND DOES NOT EXIST>\n";
}

void maistrenko::notifyUnexistingTranslation(std::ostream& out)
{
  out << "<WORD DOES NOT HAVE SUCH TRANSLATION>\n";
}

void maistrenko::notifyUnexistingFile(std::ostream& out)
{
  out << "<FILE-SOURCE DOES NOT EXIST>\n";
}

void maistrenko::notifyInvalidFile(std::ostream& out)
{
  out << "<DESTINATION FILE IS INVALID>\n";
}
