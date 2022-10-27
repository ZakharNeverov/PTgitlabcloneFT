#include "HTree.hpp"
#include <queue>
#include <fstream>
#include <algorithm>
#include "iofunctions.hpp"
#include "comExceptions.hpp"

namespace
{
  using huf_part_t = std::pair< char, std::string >;

  struct FindResultPred
  {
    bool operator()(const huf_part_t& p)
    {
      return p.second.size() == 1;
    }
  };

  struct ToLeftPred
  {
    std::unordered_map< char, std::string > result;
    bool operator()(const huf_part_t& p)
    {
      return result.find(p.first) == result.end() && *p.second.begin() == '0';
    }
  };

  struct ToRightPred
  {
    std::unordered_map< char, std::string > result;
    bool operator()(const huf_part_t& p)
    {
      return result.find(p.first) == result.end() && *p.second.begin() == '1';
    }
  };

  struct EraseFirstSymb
  {
    huf_part_t operator()(huf_part_t p)
    {
      p.second.erase(0, 1);
      return p;
    }
  };
}

neverov::HTree::HTree(const std::string& text)
{
  std::unordered_map< char, int > freq;
  for (auto&& ch: text)
  {
    freq[ch]++;
  }

  std::priority_queue< Node*, std::vector< Node* >, NodeComp > pq;

  for (auto&& pair: freq)
  {
    pq.push(new Node(pair.first, pair.second, nullptr, nullptr));
  }

  while (pq.size() != 1)
  {
    Node* left = pq.top();
    pq.pop();
    Node* right = pq.top();
    pq.pop();

    int sum = left->freq_ + right->freq_;
    pq.push(new Node('\0', sum, left, right));
  }

  root_ = pq.top();
  createEncoder(root_, "");
}

neverov::HTree::HTree(std::ifstream& fin)
{
  while (fin && !fin.eof())
  {
    std::string tempStr = "";
    std::getline(fin, tempStr);
    if (!fin.eof())
    {
      huffmanCode_.insert(splitToCharStrPair(tempStr));
    }
  }
  root_ = new Node('\0', 0, nullptr, nullptr);
  createEncoder(root_, huffmanCode_);
}

neverov::HTree::~HTree()
{
  deleteNode(root_);
}

std::string neverov::HTree::encode(const std::string& str)
{
  std::string encodedStr = "";
  for (auto&& chr: str)
  {
    try
    {
      encodedStr += huffmanCode_.at(chr);
    }
    catch (const std::out_of_range&)
    {
      throw invHufCode;
    }
  }
  return encodedStr;
}

void neverov::HTree::createEncoder(Node* root, const std::string& str)
{
  if (!root)
  {
    return;
  }

  if (!root->left_ && !root->right_)
  {
    huffmanCode_[root->ch_] = str;
  }

  createEncoder(root->left_, str + "0");
  createEncoder(root->right_, str + "1");
}

void neverov::HTree::createEncoder(Node* curNode, std::unordered_map< char, std::string > hufCode)
{
  if (hufCode.empty())
  {
    return;
  }
  std::unordered_map< char, std::string > result{};
  std::copy_if(hufCode.begin(), hufCode.end(), std::inserter(result, result.end()), ::FindResultPred{});

  bool isLeftWritten = false;
  bool isRightWritten = false;
  if (result.size() != 0)
  {
    for (auto&& p: result)
    {
      if (p.second == "0")
      {
        curNode->left_ = new Node(p.first, 0, nullptr, nullptr);
        isLeftWritten = true;
      }
      if (p.second == "1")
      {
        curNode->right_ = new Node(p.first, 0, nullptr, nullptr);
        isRightWritten = true;
      }
    }
  }

  std::unordered_map< char, std::string > tempLeftHufCode{};
  std::copy_if(hufCode.begin(), hufCode.end(), std::inserter(tempLeftHufCode, tempLeftHufCode.end()), ::ToLeftPred{result});
  std::unordered_map< char, std::string > leftHufCode{};
  std::transform(tempLeftHufCode.begin(), tempLeftHufCode.end(), std::inserter(leftHufCode, leftHufCode.end()), EraseFirstSymb{});

  std::unordered_map< char, std::string > tempRightHufCode{};
  std::copy_if(hufCode.begin(), hufCode.end(), std::inserter(tempRightHufCode, tempRightHufCode.end()), ::ToRightPred{result});
  std::unordered_map< char, std::string > rightHufCode{};
  std::transform(tempRightHufCode.begin(), tempRightHufCode.end(), std::inserter(rightHufCode, rightHufCode.end()), EraseFirstSymb{});

  if ((isLeftWritten && !tempLeftHufCode.empty()) || (isRightWritten && !tempLeftHufCode.empty()))
  {
    throw invHufCode;
  }

  if (!isLeftWritten)
  {
    curNode->left_ = new Node('\0', 0, nullptr, nullptr);
    createEncoder(curNode->left_, leftHufCode);
  }
  if (!isRightWritten)
  {
    curNode->right_ = new Node('\0', 0, nullptr, nullptr);
    createEncoder(curNode->right_, rightHufCode);
  }
}

bool neverov::HTree::addCharIfLeaf(Node*& curNode, std::string& total)
{
  if (!curNode->left_ && !curNode->right_)
  {
    total += curNode->ch_;
    curNode = root_;
    return true;
  }
  return false;
}

void neverov::HTree::deleteNode(Node* cur)
{
  if (cur->left_)
  {
    deleteNode(cur->left_);
  }
  if (cur->right_)
  {
    deleteNode(cur->right_);
  }
  delete cur;
}

std::string neverov::HTree::decode(const std::string& str)
{
  if (!root_)
  {
    throw invHufCode;
  }

  std::string total{};
  Node* cur = root_;
  for (auto&& chr: str)
  {
    addCharIfLeaf(cur, total);
    if (chr == '0')
    {
      cur = cur->left_;
    }
    else if (chr == '1')
    {
      cur = cur->right_;
    }
    else
    {
      throw invDecCode;
    }
  }
  if (!addCharIfLeaf(cur, total))
  {
    throw invHufCode;
  }
  return total;
}

neverov::HTree::Node::Node(char ch, int freq, Node* left, Node* right):
  ch_(ch),
  freq_(freq),
  left_(left),
  right_(right)
{}

bool neverov::NodeComp::operator()(HTree::Node* first, HTree::Node* second)
{
  return first->freq_ > second->freq_;
}
