#include "HuffmanTree.hpp"

#include <queue>
#include <functional>
#include <algorithm>
#include <numeric>
#include <vector>
#include <sstream>

bool yermakov::MinFreq::operator()(yermakov::HuffNode* n1, yermakov::HuffNode* n2)
{
  return n1->weight_ > n2->weight_;
}

void yermakov::pushNode(std::pair< char, std::size_t > pair, Queue& pq)
{
  pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
}

yermakov::HuffNode* yermakov::getNode(char ch, std::size_t weight, HuffNode* left, HuffNode* right)
{
  HuffNode* node = new HuffNode();
  node->ch_ = ch;
  node->weight_ = weight;
  node->left_ = left;
  node->right_ = right;
  return node;
}

void yermakov::HuffmanTree::createDicts(HuffNode* root, std::string previosBits)
{
  if (!root->right_ && !root->left_)
  {
    codeDict_.insert({root->ch_, previosBits});
    charDict_.insert({previosBits, root->ch_});
    return;
  }
  createDicts(root->left_, previosBits + "0");
  createDicts(root->right_, previosBits + "1");
}

yermakov::HuffmanTree::HuffmanTree(const CharData& text)
{
  Queue queue;
  using namespace std::placeholders;
  std::for_each(text.freqDict_.begin(), text.freqDict_.end(), std::bind(pushNode, _1, std::ref(queue)));
  while (queue.size() > 1)
  {
    HuffNode* n1 = queue.top();
    queue.pop();
    HuffNode* n2 = queue.top();
    queue.pop();
    HuffNode* newNode = getNode('\n', n2->weight_ + n1->weight_, n1, n2);
    queue.push(newNode);
  }
  root_ = queue.top();
  queue.pop();
  createDicts(root_, "");
}

yermakov::CharData yermakov::HuffmanTree::compress(const CharData& text) const
{
  std::string code = std::string();
  // std::accumulate(text.text_.begin(), text.text_.end(), std::string());
  for (size_t i = 0; i < text.text_.length(); i++)
  {
    code += codeDict_.at(text.text_[i]);
    code += i == text.text_.length() - 1 ? "\n" : " ";
  }
  CharData newText;
  newText.language_ = text.language_;
  newText.text_ = code;
  return newText;
}

yermakov::CharData yermakov::HuffmanTree::decompress(const CharData& text) const
{
  std::string code = text.text_;
  std::string textString = std::string();
  std::stringstream stream(code);
  std::string instring = std::string();
  while (stream >> instring)
  {
    textString += charDict_.at(instring);
  }
  CharData newText;
  newText.language_ = text.language_;
  newText.text_ = textString;
  return newText;
}
