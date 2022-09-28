#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP

#include <queue>
#include <cstddef>
#include <memory>

#include "CharDataStruct.hpp"

namespace yermakov
{
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
    struct HuffNode
    {
      HuffNode() = default;
      HuffNode(char, std::size_t, std::shared_ptr< HuffNode >, std::shared_ptr< HuffNode >);
      char ch_;
      std::size_t weight_;
      std::shared_ptr< HuffNode > right_;
      std::shared_ptr< HuffNode > left_;
    };

    using NodePtr = std::shared_ptr< HuffmanTree::HuffNode >;

    struct MinFreq
    {
      bool operator()(NodePtr n1, NodePtr n2)
      {
        return n1->weight_ > n2->weight_;
      }
    };

    using Queue = std::priority_queue< NodePtr, std::vector< NodePtr >, MinFreq >;

    void pushNode(std::pair< char, std::size_t >, Queue&);
    void CreateRecursiveCodeDict(CodeDict&, NodePtr, const std::string&);
    NodePtr makeHuffTree(const CharData&);
    CodeDict createCodeDict(NodePtr);
    CharDict createCharDict();
  };
}

#endif
