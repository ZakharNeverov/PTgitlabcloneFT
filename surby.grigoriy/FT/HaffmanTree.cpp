#include "HaffmanTree.hpp"
surby::Node* surby::getNode(char symb, int count, Node* left, Node* right)
{
  Node* node = new Node();
  node->symb = symb;
  node->count = count;
  node->left = left;
  node->right = right;
  return node;
}

bool surby::compare::operator()(Node* left, Node* right)
{
  return left->count > right->count;
}
