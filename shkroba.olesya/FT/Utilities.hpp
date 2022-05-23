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
  void doAddWordsFromAnother(Dictionary& source, const Dictionary& extra, std::ostream& out);
  void doCreateFromOneTranslate(const Dictionary& dictionary, std::ostream& out);
  void doCreateFromUniqueWords(const Dictionary& source, const Dictionary& extra, std::ostream& out);
  void doCommonDictionary(const std::vector< Dictionary > &dictionaries, size_t n);

  void help(std::ostream& out);
  void createTestDictionaries(Dictionary& d1, Dictionary& d2, Dictionary& d3, Dictionary& d4, Dictionary& d5);
  void testCommandSystem(Dictionary&  d1, Dictionary&  d2, Dictionary&  d3, Dictionary&  d4, Dictionary&  d5);

}
#endif
