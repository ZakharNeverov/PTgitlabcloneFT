#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Dictionary.hpp"

using map = std::map< std::string, std::list< std::string > >;
using dictsArray = std::map< std::string, roletskaya::Dictionary >;

namespace roletskaya
{
  bool isCorrectDictName(std::string& name, dictsArray& dictsArray);
  bool pushDictToArray(std::string& name, std::string fileName, dictsArray& dictsArray);

  map merge(std::vector< Dictionary >& dictsVector);
  void complement(std::vector< Dictionary >& dictsVector);
  bool equals(std::vector< Dictionary >& dictsVector);
}
#endif
