#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Dictionary.hpp"

using map = std::map< std::string, std::list< std::string > >;

namespace roletskaya
{
  std::vector< map > getFiles(std::string& files);
  map merge(std::vector< map >& dictsVector);
  map complement(std::vector< map >& dictsVector);
  bool equals(std::vector< map >& dictsVector);
  bool checkResults(std::string& outFileName, std::string& ResultsFileName);
}
#endif
