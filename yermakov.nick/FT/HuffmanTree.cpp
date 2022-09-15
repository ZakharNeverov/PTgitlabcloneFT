#include "HuffmanTree.hpp"

#include <queue>
#include <functional>
#include <algorithm>
#include <numeric>
#include <sstream>

yermakov::HuffNode::HuffNode(char ch, std::size_t weight, HuffNode* left, HuffNode* right):
  ch_(ch),
  weight_(weight),
  left_(left),
  right_(right)
{

}

bool yermakov::MinFreq::operator()(yermakov::HuffNode* n1, yermakov::HuffNode* n2)
{
  return n1->weight_ > n2->weight_;
}

void yermakov::pushNode(std::pair< char, std::size_t > pair, Queue& pq)
{
  pq.push(new HuffNode(pair.first, pair.second, nullptr, nullptr));
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

yermakov::HuffmanTree::HuffmanTree():
  codeDict_(),
  charDict_()
{
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
    HuffNode* newNode = new HuffNode('\n', n2->weight_ + n1->weight_, n1, n2);
    queue.push(newNode);
  }
  HuffNode* root = queue.top();
  queue.pop();
  createDicts(root, "");
  destroyRecursive(root);
}

namespace
{
  struct CodeCreator
  {
    CodeCreator(const std::map< char, std::string >& codeDict):
      codeDict_(codeDict)
    {}

    std::string operator()(std::string a, const char b)
    {
      return std::move(a) + codeDict_.at(b) + " ";
    }
    std::map< char, std::string > codeDict_;
  };
}

yermakov::CharData yermakov::HuffmanTree::compress(const CharData& text) const
{
  CodeCreator creator(codeDict_);
  std::string code = std::accumulate(text.text_.begin(), text.text_.end(), std::string(), creator);
  CharData newText;
  newText.language_ = text.language_;
  newText.text_ = code.substr(0, code.find_last_of(" "));
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

void yermakov::destroyRecursive(HuffNode* node)
{
  if (node)
  {
    destroyRecursive(node->left_);
    destroyRecursive(node->right_);
    delete node;
  }
}
