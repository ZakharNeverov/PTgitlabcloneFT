#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <string>
#include <queue>
#include <unordered_map>
#include "HaffmanTree.hpp"
#include "EncodeDecode.hpp"
namespace surby
{
  std::string getBestDict(std::string&);
  std::unordered_map< char, std::string > getDictionary(std::string&);
  std::unordered_map< std::string, char > getReverseDictionary(std::string&);
  std::string getBestDictCompare(std::string, std::pair< char, std::string >);
}

#endif
