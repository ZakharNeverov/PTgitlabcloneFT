#ifndef HAFFMAN_HPP
#define HAFFMAN_HPP
#include <unordered_map>
#include <string>
#include <iosfwd>

struct Node
{
  char ch;
  int freq;
  Node* left, * right;
};
struct comp
{
  bool operator()(Node* l, Node* r);
};
void decode(Node* root, int& index, std::string str);
void encode(Node* root, std::string str, std::unordered_map<char, std::string>& huffmanCode);
Node* getNode(char ch, int freq, Node* left, Node* right);
#endif
