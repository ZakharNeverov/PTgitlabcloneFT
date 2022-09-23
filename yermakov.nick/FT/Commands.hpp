#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <map>

#include "TextStruct.hpp"

namespace yermakov
{
  using TextDict = std::map< std::string, yermakov::Text >;

  void doGet(yermakov::TextDict&, std::string&);
  void doWrite(const yermakov::TextDict&, std::string&);
  void doCalculateSize(const yermakov::TextDict&, std::string&);
  void doPrint(const yermakov::TextDict&, std::string&);
  void doCompress(yermakov::TextDict&, std::string&);
  void doDecompress(yermakov::TextDict&, std::string&);
  void doEfficiency(const yermakov::TextDict&, std::string&);
  void doConcat(yermakov::TextDict&, std::string&);
  void doCut(yermakov::TextDict&, std::string&);
}

#endif
