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

  class HuffmanTree
  {
  public:
    HuffmanTree();
    HuffmanTree(const CharData&);
    CharData compress(const CharData&) const;
    CharData decompress(const CharData&) const;

  private:
    std::map< char, std::string > codeDict_;
    std::map< std::string, char > charDict_;
    void CreateRecursiveCodeDict(std::map< char, std::string >&, NodePtr, const std::string&);
    NodePtr makeHuffTree(const CharData& text);
    std::map< char, std::string > createCodeDict(NodePtr root);
    std::map< std::string, char > createCharDict();
  };
}

#endif
