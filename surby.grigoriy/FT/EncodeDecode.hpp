#ifndef ENCODEDECODE_HPP
#define ENCODEDECODE_HPP
#include <string>
#include <unordered_map>
#include "HaffmanTree.hpp"
#include "inputProcess.hpp"
#include "Dictionary.hpp"
namespace surby
{
  std::string encode(std::string&, std::string&);
  std::string decode(std::string&, std::string&);

  void encodeHaffmanTree(Node* head, std::string str, std::unordered_map< char, std::string >& huffmanCode);
}
#endif
