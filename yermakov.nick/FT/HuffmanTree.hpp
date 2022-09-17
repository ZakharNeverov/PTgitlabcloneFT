#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP

#include <queue>
#include <cstddef>
#include <memory>

#include "CharDataStruct.hpp"

namespace yermakov
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

  using NodePtr = std::shared_ptr< yermakov::HuffNode >;

  struct MinFreq
  {
    bool operator()(NodePtr, NodePtr);
  };

  using Queue = std::priority_queue< NodePtr, std::vector< NodePtr >, MinFreq >;

  void pushNode(std::pair< char, std::size_t >, Queue&);

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
    void createDicts(NodePtr, std::string);
  };
}

#endif
