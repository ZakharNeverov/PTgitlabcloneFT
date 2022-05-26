#ifndef IO_ACTIONS_HPP
#define IO_ACTIONS_HPP

#include <iosfwd>
#include "DictionaryAliases.hpp"

namespace maistrenko
{
  bool getNextWord(std::string& fromString, char delimiter, std::string& toString);
  bool getWordFromStream(std::istream&, std::pair< EngW, RusS >&);
  bool getWordFromString(std::string&, std::pair< EngW, RusS >&);
  void getDictFromStream(std::istream&, Dict&);

  void outputTranslations(const Dict&, const EngW&, std::ostream&);
  void outputDict(const Dict&, std::ostream&);

  bool loadFromFile(const std::string&, Dict&);
  bool saveInFile(const std::string&, const Dict&);
}

#endif
