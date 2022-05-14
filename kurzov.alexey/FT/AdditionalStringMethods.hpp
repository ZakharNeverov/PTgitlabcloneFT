#ifndef ADDITIONAL_STRING_METHOD_HPP_T1
#define ADDITIONAL_STRING_METHOD_HPP_T1

#include <iosfwd>
#include <string>

namespace kurzov
{
  std::string getNextWord(std::string& str, char delimeter = ' ');
  std::string getNameOfFile(std::string);
}

#endif
