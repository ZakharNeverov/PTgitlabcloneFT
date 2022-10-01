#ifndef RGR_ENG_RUS_UTILITIES_HPP
#define RGR_ENG_RUS_UTILITIES_HPP

#include <iosfwd>
#include <set>
#include <vector>
#include "Dictionary.hpp"

namespace shkroba
{
  Dictionary createCommonDictionary(std::vector< Dictionary >& common);
  std::ostream& operator<<(std::ostream& out, std::set< std::string >& set);
  void doPrintDictionary(const Dictionary& dictionary, std::ostream& out);
  void doPrintInFile(const Dictionary& dictionary, const std::string& fileName);
  void doSize(const Dictionary& dictionary, std::ostream& out);
  void doFindWord(const Dictionary& dictionary, std::string letter, std::ostream& out);
  std::string nextWord(std::string& str);
  std::vector< Dictionary > createDictionariesFromFile(std::istream& fin);
}

#endif
