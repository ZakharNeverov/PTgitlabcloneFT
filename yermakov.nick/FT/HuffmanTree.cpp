#include "HuffmanTree.hpp"

#include <queue>
#include <functional>
#include <algorithm>
#include <numeric>

#include "BasedParseFunction.hpp"

yermakov::HuffmanTree::HuffNode::HuffNode(char ch, std::size_t weight, NodePtr left, NodePtr right):
  ch_(ch),
  weight_(weight),
  left_(left),
  right_(right)
{}

void yermakov::HuffmanTree::pushNode(std::pair< char, std::size_t > pair, yermakov::HuffmanTree::Queue& pq)
{
  pq.push(std::make_shared< HuffmanTree::HuffNode >(pair.first, pair.second, nullptr, nullptr));
}

yermakov::HuffmanTree::NodePtr yermakov::HuffmanTree::makeHuffTree(const CharData& text)
{
  Queue queue;
  using namespace std::placeholders;
  std::for_each(text.freqDict_.begin(), text.freqDict_.end(), std::bind(&HuffmanTree::pushNode, this, _1, std::ref(queue)));
  while (queue.size() > 1)
  {
    NodePtr n1 = queue.top();
    queue.pop();
    NodePtr n2 = queue.top();
    queue.pop();
    NodePtr newNode = std::make_shared< HuffNode >('\n', n2->weight_ + n1->weight_, n1, n2);
    queue.push(newNode);
  }
  NodePtr root = queue.top();
  queue.pop();
  return root;
}

yermakov::CodeDict yermakov::HuffmanTree::createCodeDict(NodePtr root)
{
  CodeDict codeDict;
  this->CreateRecursiveCodeDict(codeDict, root, "");
  return codeDict;
}

yermakov::CharDict yermakov::HuffmanTree::createCharDict()
{
  CharDict charDict;
  auto iter = codeDict_.begin();
  while (iter != codeDict_.end())
  {
    charDict.insert({(*iter).second, (*iter).first});
    ++iter;
  }
  return charDict;
}

void yermakov::HuffmanTree::CreateRecursiveCodeDict(CodeDict& codeDict, NodePtr root, const std::string& previosBits)
{
  if (!root->right_ && !root->left_)
  {
    codeDict.insert({root->ch_, previosBits});
    return;
  }
  CreateRecursiveCodeDict(codeDict, root->left_, previosBits + "0");
  CreateRecursiveCodeDict(codeDict, root->right_, previosBits + "1");
}

yermakov::HuffmanTree::HuffmanTree():
  codeDict_(),
  charDict_()
{}

yermakov::HuffmanTree::HuffmanTree(const CharData& text):
  codeDict_(createCodeDict(makeHuffTree(text))), charDict_(createCharDict())
{}

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
  while (!code.empty())
  {
    std::string instring = getAndEraseWord(code);
    textString += charDict_.at(instring);
  }
  CharData newText;
  newText.language_ = text.language_;
  newText.text_ = textString;
  return newText;
}
