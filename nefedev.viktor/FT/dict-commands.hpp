#ifndef DICT_COMMANDS_HPP
#define DICT_COMMANDS_HPP

#include <string>
#include <map>
#include <unordered_map>
#include <iosfwd>

using FreqDictionary = std::map< std::string, unsigned int >;
using DictionaryArray = std::unordered_map<std::string, FreqDictionary >;
using WordAndFreq = std::pair< std::string, unsigned int >;

namespace nefedev
{
  void insert(std::istream&, std::ostream&, DictionaryArray&);
  void readText(std::istream&, std::ostream&, DictionaryArray&);
  void readFile(std::istream&, std::ostream&, DictionaryArray&);
  void deleteWord(std::istream&, std::ostream&, DictionaryArray&);
  void deleteAllWords(std::istream&, std::ostream&, DictionaryArray&);
  void wordNumber(std::istream&, std::ostream&, DictionaryArray&);
  void printAlphabetic(std::istream&, std::ostream&, DictionaryArray&);
  void printFrequency(std::istream&, std::ostream&, DictionaryArray&);
  void find(std::istream&, std::ostream&, DictionaryArray&);
  void findAndPrint(std::istream&, std::ostream&, DictionaryArray&);
  void merge(std::istream&, std::ostream&, DictionaryArray&);
  void saveDict(std::istream&, std::ostream&, DictionaryArray&);
  void loadDict(std::istream&, std::ostream&, DictionaryArray&);

  void outInvalidCommand(std::ostream& out);
  void cleanIStream(std::istream& in);
}

#endif
