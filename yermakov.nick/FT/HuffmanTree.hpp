#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP

#include <queue>
#include <cstddef>
#include <memory>

#include "CharDataStruct.hpp"

namespace yermakov
{
  namespace
  {
    struct HuffNode
    {
      HuffNode() = default;
      HuffNode(char, std::size_t, std::shared_ptr< yermakov::HuffNode >, std::shared_ptr< yermakov::HuffNode >);
      char ch_;
      std::size_t weight_;
      std::shared_ptr< yermakov::HuffNode > right_;
      std::shared_ptr< yermakov::HuffNode > left_;
    };
  }
  using NodePtr = std::shared_ptr< yermakov::HuffNode >;
  using CharDict = std::map< std::string, char >;
  using CodeDict = std::map< char, std::string >;

  class HuffmanTree
  {
  public:
    HuffmanTree();
    HuffmanTree(const CharData&);
    CharData compress(const CharData&) const;
    CharData decompress(const CharData&) const;

  private:
    CodeDict codeDict_;
    CharDict charDict_;
    void CreateRecursiveCodeDict(CodeDict&, NodePtr, const std::string&);
    NodePtr makeHuffTree(const CharData& text);
    CodeDict createCodeDict(NodePtr root);
    CharDict createCharDict();
  };
}

#endif
