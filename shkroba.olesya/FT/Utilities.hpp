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

  void doPrintDictionary(Dictionary& dictionary, std::ostream& out);
  void doSize(Dictionary& dictionary, std::ostream& out);
  void doFindWord(Dictionary& dictionary, char letter, std::ostream& out);
  void doAddWordsFromAnother(Dictionary& source, Dictionary& extra, std::ostream& out);
  void doCreateFromOneTranslate(Dictionary& dictionary, std::ostream& out);
  void doCreateFromUniqueWords(Dictionary& source, Dictionary& extra, std::ostream& out);
  void doCommonDictionary(std::vector< Dictionary > dictionaries, size_t n);

  void help(std::ostream& out);
  void createTestDictionaries(Dictionary& d1, Dictionary& d2, Dictionary& d3, Dictionary& d4, Dictionary& d5);
  void testCommandSystem(Dictionary&  d1, Dictionary&  d2, Dictionary&  d3, Dictionary&  d4, Dictionary&  d5);

}
#endif


