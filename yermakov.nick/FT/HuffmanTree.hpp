#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP

#include <queue>
#include <cstddef>

#include "CharDataStruct.hpp"

namespace yermakov
{
  struct HuffNode
  {
    HuffNode() = default;
    HuffNode(char, std::size_t, HuffNode*, HuffNode*);
    char ch_;
    std::size_t weight_;
    HuffNode* right_;
    HuffNode* left_;
  };

  struct MinFreq
  {
    bool operator()(yermakov::HuffNode*, yermakov::HuffNode*);
  };

  using Queue = std::priority_queue< HuffNode*, std::vector< HuffNode* >, MinFreq >;

  void pushNode(std::pair< char, std::size_t >, Queue&);
  void DestroyRecursive(HuffNode*);

  class HuffmanTree
  {
  public:
    HuffmanTree();
    HuffmanTree(const CharData&);
    ~HuffmanTree();
    CharData compress(const CharData&) const;
    CharData decompress(const CharData&) const;
  private:
    HuffNode* root_;
    std::map< char, std::string > codeDict_;
    std::map< std::string, char > charDict_;
    void createDicts(HuffNode*, std::string);
  };
}

#endif
