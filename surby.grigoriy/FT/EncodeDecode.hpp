#ifndef ENCODEDECODE_HPP
#define ENCODEDECODE_HPP
#include <string>
#include <unordered_map>
#include "HaffmanTree.hpp"
#include "inputProcess.hpp"
#include "Dictionary.hpp"
namespace surby
{
  using map_str_char = std::unordered_map< std::string, char >;

  std::string encode(std::string&, std::string&);
  std::string decode(std::string&, std::string&);

  void encodeCompare(char, std::string&, std::unordered_map< char, std::string >&);
  void decodeCompare(char, std::string&, std::string&, map_str_char&);
  void encodeHaffmanTree(Node*, std::string, std::unordered_map< char, std::string >&);
}
#endif
