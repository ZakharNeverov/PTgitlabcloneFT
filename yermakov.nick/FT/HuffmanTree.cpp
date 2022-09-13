#include "HuffmanTree.hpp"

#include <queue>
#include <functional>
#include <algorithm>
#include <numeric>
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

yermakov::HuffmanTree::HuffmanTree():
  root_(nullptr),
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
    HuffNode* newNode = getNode('\n', n2->weight_ + n1->weight_, n1, n2);
    queue.push(newNode);
  }
  root_ = queue.top();
  queue.pop();
  createDicts(root_, "");
}

yermakov::HuffmanTree::~HuffmanTree()
{
  DestroyRecursive(root_);
}

namespace
{
  struct CodeCreator
  {
    CodeCreator(const std::map< char, std::string >& codeDict):
      codeDict_(codeDict)
    {}

    std::string operator()(std::string a, std::string b)
    {
      return std::move(a) + b + " ";
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

void yermakov::DestroyRecursive(HuffNode* node)
{
  if (node)
  {
    DestroyRecursive(node->left_);
    DestroyRecursive(node->right_);
    delete node;
  }
}
