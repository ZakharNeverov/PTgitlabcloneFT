#ifndef UTILITES_HPP
#define UTILITES_HPP
#include <string>

namespace roletskaya
{
  std::string getElem(std::string& string);
  std::string getNameFromFile(std::string& string);
  bool checkResults(std::string& outFileName, std::string& resultsFileName);
}
#endif
