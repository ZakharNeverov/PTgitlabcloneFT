#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <string>
#include <queue>
#include <unordered_map>
#include "HaffmanTree.hpp"
#include "EncodeDecode.hpp"
namespace surby
{
  using priorityQueue = std::priority_queue< Node*, std::vector< Node* >, compare >;

  std::string getBestDict(std::string&);
  std::unordered_map< char, std::string > getDictionary(std::string&);
  std::unordered_map< std::string, char > getReverseDictionary(std::string&);
  std::string getBestDictCompare(std::string, std::pair< char, std::string >);

  void createPriorityQueueCompare(std::pair< const char, int >, std::priority_queue< Node*, std::vector< Node* >, compare >&);
  void createDictCompare(char, std::unordered_map< char, int >&);
}
#endif
