#ifndef RGR_ENG_RUS_UTILITIES_HPP
#define RGR_ENG_RUS_UTILITIES_HPP

#include <iostream>
#include <set>
#include <vector>
#include "Dictionary.hpp"
namespace shkroba
{
  Dictionary createFromOneTranslate(const Dictionary& dictionary);
  Dictionary createCommonDictionary(std::vector< Dictionary >& common);
  Dictionary createFromUniqueWords(const Dictionary& d1, const Dictionary& d2);

  std::ostream& operator<<(std::ostream& out, std::set< std::string >& set);

  void doPrintDictionary(const Dictionary& dictionary, std::ostream& out);
  void doSize(const Dictionary& dictionary, std::ostream& out);
  void doFindWord(const Dictionary& dictionary, char letter, std::ostream& out);
  void doCommonForTwo(const Dictionary& source, const Dictionary& extra, std::ostream& out);
  void doOneTranslate(const Dictionary& dictionary, std::ostream& out);
  void doCreateFromUniqueWords(const Dictionary& source, const Dictionary& extra, std::ostream& out);
  std::string nextWord(std::string &str);
  std::vector<Dictionary> createDictionariesFromFile(std::istream &fin);
}
#endif
