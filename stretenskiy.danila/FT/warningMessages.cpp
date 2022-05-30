#include "warningMessages.hpp"

#include <iostream>

namespace stretenskiy
{
  std::ostream &informBadArgs(std::ostream &out)
  {
    return out << "Oh my God, I don't see the file\n";
  }

  std::ostream &informNotOpenFile(std::ostream &out)
  {
    return out << "I can't open the file, I have paws\n";
  }

  std::ostream &informNotFindCommand(std::ostream &out)
  {
    return out << "Command not found\n";
  }

  std::ostream &informUnexistDict(std::ostream &out)
  {
    return out << "This name don't find in dict, create him\n";
  }

  std::ostream &informExistDict(std::ostream &out)
  {
    return out << "This dict has already\n";
  }

  std::ostream &informEnterWord(std::ostream &out)
  {
    return out << "Enter the word\n";
  }

  std::ostream &informNotFindWord(std::ostream &out)
  {
    return  out << "The right word was not found in the dictionary\n";
  }

  std::ostream &informInsufficientData(std::ostream &out)
  {
    return out << "Insufficient data\n";
  }
}