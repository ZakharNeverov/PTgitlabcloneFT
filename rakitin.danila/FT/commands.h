#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "dictionary_usings.h"

namespace rakitin {

  void add(dictionaries& data, const std::string& dataname, const std::string& path);
  ref_finder findWord(const dictionaries& data, const std::string& dataname, const std::string& word);
  void printAll(const dictionaries& data, const std::string& dataname, std::ostream& stream);
  void merge(dictionaries& data, const std::string& dataname1, const std::string& dataname2);
  void intersection(dictionaries& data, const std::string& dataname1, const std::string& dataname2);
  double similarity(const dictionaries& data, const std::string& dataname1, const std::string& dataname2);
  void deleteDictionary(dictionaries& data, const std::string& dataname1);
  void rename(dictionaries& data, const std::string& dataname1, const std::string& newName);
  void printAllDatanames(const dictionaries& data, std::ostream& stream);
  void clear(dictionaries& data, const std::string& dataname);

  rakitin::dictionary getDictionaryFromData(const dictionaries& data, const std::string& dataname);
}

#endif
