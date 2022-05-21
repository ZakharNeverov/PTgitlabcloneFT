#include "HaffmanTree.hpp"
surby::Node* surby::getNode(char simv, int count, Node* left, Node* right)
{
  Node* node = new Node();

  node->simv = simv;
  node->count = count;
  node->left = left;
  node->right = right;

  return node;
}

bool surby::compare::operator()(Node* l, Node* r)
{
  return l->count > r->count;
}
