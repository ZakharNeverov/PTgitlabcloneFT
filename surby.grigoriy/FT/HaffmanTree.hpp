#ifndef HAFFMANTREE_HPP
#define HAFFMANTREE_HPP

namespace surby
{
  struct Node
  {
    char symb;
    int count;
    Node* left, * right;
  };

  struct compare
  {
    bool operator()(Node*, Node*);
  };

  Node* getNode(char, int, Node*, Node*);
}
#endif
