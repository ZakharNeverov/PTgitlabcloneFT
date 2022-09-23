#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <map>

#include "TextStruct.hpp"

namespace yermakov
{
  using TextDict = std::map< std::string, yermakov::Text >;

  void doGet(std::ostream&, yermakov::TextDict&, std::string&);
  void doWrite(std::ostream&, const yermakov::TextDict&, std::string&);
  void doCalculateSize(std::ostream&, const yermakov::TextDict&, std::string&);
  void doPrint(std::ostream&, const yermakov::TextDict&, std::string&);
  void doCompress(std::ostream&, yermakov::TextDict&, std::string&);
  void doDecompress(std::ostream&, yermakov::TextDict&, std::string&);
  void doEfficiency(std::ostream&, const yermakov::TextDict&, std::string&);
  void doConcat(std::ostream&, yermakov::TextDict&, std::string&);
  void doCut(std::ostream&, yermakov::TextDict&, std::string&);
}

#endif
