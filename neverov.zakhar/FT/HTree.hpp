#ifndef H_TREE_HPP
#define H_TREE_HPP

#include <string>
#include <unordered_map>

namespace neverov
{
  class HTree
  {
  public:
    HTree(const std::string&);
    HTree(std::ifstream&);
    ~HTree();
    std::string encode(const std::string&);
    std::string decode(const std::string&);

  private:
    struct Node
    {
      Node(char ch, int freq, Node* left, Node* right);

      char ch_;
      int freq_;
      Node* left_;
      Node* right_;
    };
    friend struct NodeComp;

    std::unordered_map< char, std::string > huffmanCode_;
    Node* root_;

    friend void outputHuffmanCode(std::ostream&, const HTree&);
    void createEncoder(Node*, const std::string&);
    void createEncoder(Node*, std::unordered_map< char, std::string >);
    bool addCharIfLeaf(Node*&, std::string&);
    void deleteNode(Node*);
  };

  struct NodeComp
  {
    bool operator()(HTree::Node* first, HTree::Node* second);
  };
}

#endif
