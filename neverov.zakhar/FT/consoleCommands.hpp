#ifndef CONSOLE_COMMANDS_HPP
#define CONSOLE_COMMANDS_HPP

#include "dataTypes.hpp"

namespace neverov
{
  void createText(std::string&, texts_t&, std::istream&);
  void encode(std::string&, texts_t&);
  void decode(std::string&, texts_t&);
  void getText(std::string&, const texts_t&, std::ostream&);
  void mergeText(std::string&, texts_t&);
  void getCompDiff(std::string&, const texts_t&, std::ostream&);
  void getDict(std::string&, const texts_t&);
}

#endif
