#include "Haffman.hpp"
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>

Node* getNode(char ch, int freq, Node* left, Node* right)
{
  Node* node = new Node();

  node->ch = ch;
  node->freq = freq;
  node->left = left;
  node->right = right;

  return node;
}

bool comp::operator()(Node* l, Node* r)
{
  return l->freq > r->freq;
}

void encode(Node* root, std::string str, std::unordered_map<char, std::string>& huffmanCode)
{
  if (root == nullptr)
    return;

  // found a leaf node
  if (!root->left && !root->right) {
    huffmanCode[root->ch] = str;
  }

  encode(root->left, str + "0", huffmanCode);
  encode(root->right, str + "1", huffmanCode);
}

void decode(Node* root, int& index, std::string str)
{
  if (root == nullptr) {
    return;
  }

  // found a leaf node
  if (!root->left && !root->right)
  {
    std::cout << root->ch;
    return;
  }

  index++;

  if (str[index] == '0')
    decode(root->left, index, str);
  else
    decode(root->right, index, str);
}
