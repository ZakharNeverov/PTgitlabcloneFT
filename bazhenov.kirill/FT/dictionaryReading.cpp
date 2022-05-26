#include "dictionaryReading.hpp"
#include <algorithm>
#include <fstream>

namespace {
  bool myIsalpha(char ch)
  {
    return std::isalpha(static_cast<unsigned char>(ch));
  }

  bool isWordWithoutSymbols(const std::string& str)
  {
    return std::all_of(str.begin(), str.end(), myIsalpha);
  }

  void clearWord(std::string& word)
  {
    std::string symbols = ".,:-";
    if (symbols.find(word.front()) != std::string::npos) {
      word.erase(word.begin());
    }
    if (symbols.find(word.back()) != std::string::npos) {
      word.erase(word.size() - 1);
    }
  }
}

std::string bazhenov::createName(const std::string& str)
{
  return str.substr(0, str.find('.'));
}

bazhenov::freq bazhenov::readDictionary(std::istream& file)
{
  bazhenov::freq frequencyDictionary;
  std::string word = "";
  while (file >> word) {
    readWord(word, frequencyDictionary);
  }
  return frequencyDictionary;
}

void bazhenov::readWord(std::string& word, bazhenov::freq& dictionary)
{
  clearWord(word);
  if (isWordWithoutSymbols(word)) {
    std::transform(word.begin(), word.end(), word.begin(), tolower);
    if (dictionary.find(word) != dictionary.end()) {
      ++dictionary.at(word);
    } else {
      dictionary.insert(std::make_pair(word, 1));
    }
  }
}
