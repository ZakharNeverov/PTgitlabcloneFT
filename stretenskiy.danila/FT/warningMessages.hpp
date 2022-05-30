#ifndef WARNING_MESSAGES_HPP
#define WARNING_MESSAGES_HPP

#include <iosfwd>

namespace stretenskiy
{
  std::ostream &informBadArgs(std::ostream &);
  std::ostream &informNotOpenFile(std::ostream &);
  std::ostream &informNotFindCommand(std::ostream &);
  std::ostream &informUnexistDict(std::ostream &);
  std::ostream &informExistDict(std::ostream &);
  std::ostream &informEnterWord(std::ostream &);
  std::ostream &informNotFindWord(std::ostream &);
  std::ostream &informInsufficientData(std::ostream &);
}

#endif
