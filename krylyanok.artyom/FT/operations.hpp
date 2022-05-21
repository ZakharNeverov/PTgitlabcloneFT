#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include <string>
#include <vector>

namespace krylyanok
{
  void correctVector(std::vector< std::string >& vec);
  bool correctString(std::string& string);
  bool makeDict(std::string& nameFile, std::vector< std::string >&);
}
#endif
