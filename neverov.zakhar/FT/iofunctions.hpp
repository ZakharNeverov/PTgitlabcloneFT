#ifndef IO_FUNCTIONS_HPP
#define IO_FUNCTIONS_HPP

#include <string>
#include "HTree.hpp"
#include "dataTypes.hpp"

namespace neverov
{
  bool getNextWord(std::string&, std::string&);
  std::pair< char, std::string > splitToCharStrPair(std::string);
  void inputText(std::istream&, std::string&);
  void outputHuffmanCode(std::ostream&, const HTree&);
  args_t getArgs(std::string&, int, int);
}

#endif
