#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include <string>
#include <vector>
#include "dictionary.hpp"

namespace krylyanok
{
  void correctVector(std::vector< std::string >& vec);
  bool correctString(std::string& string);
  void makeDict(std::string& nameFile, std::vector< std::string >&);
  void makeTech(std::string& nameFile, krylyanok::NameMap&);
  void createDict(std::vector< std::string >&, krylyanok::NameMap&);
}
#endif
