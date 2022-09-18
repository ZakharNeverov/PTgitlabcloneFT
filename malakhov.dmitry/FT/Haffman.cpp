#include "Haffman.hpp"

#include <cmath>
#include <iostream>

#include "Stack.hpp"
#include "Queue.hpp"

namespace
{
  using malakhov::Dictionary;
  using malakhov::Queue;

  using BitmaskT = malakhov::Haffman::BitmaskT;
  using FrequencyT = malakhov::Dictionary< unsigned char, size_t >;

  const unsigned char BYTE_SIZE = 8;

  class HaffmanTree
  {
    using KeyT = std::string;
    using ValueT = size_t;
    using PairT = std::pair< KeyT, ValueT >;

    struct Node
    {
    public:
      Node(const Node&) = delete;

      Node(const Node& instance, Node* up):
        stored(instance.stored),
        up(up),
        right(instance.right ? new Node{*instance.right, this} : nullptr),
        left(instance.left ? new Node{*instance.left, this} : nullptr)
      {}

      Node(const PairT& pair, Node* up = nullptr, Node* right = nullptr, Node* left = nullptr):
        stored(pair),
        up(up),
        right(right),
        left(left)
      {}

      ~Node()
      {
        if (right)
        {
          delete right;
          right = nullptr;
        }
        if (left)
        {
          delete left;
          left = nullptr;
        }
      }

      PairT stored;
      Node* up;
      Node* right;
      Node* left;
    };

  public:
    ~HaffmanTree() noexcept
    {
      if (crown_)
      {
        delete crown_;
        crown_ = nullptr;
      }
    }

    static HaffmanTree construct(const FrequencyT& frequency)
    {
      Queue< Node* > nodes;
      nodes.putIn(new Node{{std::string{} + '\0', 1}});
      for (auto it = frequency.getBegin(); it != frequency.getEnd(); ++it)
      {
        const std::string character = std::string{} + static_cast< char >(it->first);
        nodes.putIn(new Node{{character, it->second}});
      }
      while (nodes.getUsedSpace() > 1)
      {
        Node* n1 = new Node{*nodes.getFirst(), nullptr};
        const PairT& stored1 = n1->stored;
        nodes.popOut();

        Node* n2 = new Node{*nodes.getFirst(), nullptr};
        const PairT& stored2 = n2->stored;
        nodes.popOut();

        Node* n3 = new Node{{stored1.first + stored2.first, stored1.second + stored2.second}, nullptr, n2, n1};
        nodes.putIn(n3);
      }
      return HaffmanTree{nodes.getFirst()};
    }

    BitmaskT getBitMask() const
    {
      BitmaskT result;
      if (crown_)
      {
        getBitMaskImpl(crown_, result, 0);
      }
      return result;
    }

  private:
    explicit HaffmanTree(Node* node) noexcept:
      crown_(node)
    {}

    static void getBitMaskImpl(const Node* n, BitmaskT& result, const unsigned short bitmask)
    {
      if (n->left)
      {
        getBitMaskImpl(n->left, result, bitmask << 1);
      }
      if (n->right)
      {
        getBitMaskImpl(n->right, result, (bitmask << 1) + 1);
      }
      else if (!n->left && !n->right)
      {
        result.insert(n->stored.first[0], bitmask);
      }
    }

    Node* crown_;
  };

  FrequencyT getFrequency(const std::string& str)
  {
    FrequencyT result;
    for (auto&& letter : str)
    {
      auto it = result.find(letter);
      if (it == result.getEnd())
      {
        result.insert(letter, 1);
      }
      else
      {
        ++(it->second);
      }
    }
    return result;
  }

  BitmaskT getReversedMask(const BitmaskT& oldBitmask)
  {
    BitmaskT result;
    for (auto it = oldBitmask.getBegin(); it != oldBitmask.getEnd(); ++it)
    {
      result.insert(it->second, it->first);
    }
    return result;
  }

  size_t getBits(const size_t a, const unsigned char bitsCount, const unsigned char skipLast) noexcept
  {
    const size_t mask = ((1 << bitsCount) - 1) << skipLast;
    return (a & mask) >> skipLast;
  }

  size_t getBitsPerEncodedLetter(const BitmaskT& bitmask) noexcept
  {
    return static_cast< size_t >(std::log2(bitmask.getSize()) + 1);
  }

  void flush(const size_t buf, const unsigned char bitsNotToFlush, std::string& str)
  {
    const unsigned char encodedChar = getBits(buf, BYTE_SIZE, bitsNotToFlush);
    str.push_back(encodedChar);
  }

  std::ostream& print(std::ostream& os, const malakhov::Haffman::BitmaskT& bitmask)
  {
    const static char delim = std::numeric_limits< char >::max();

    std::ostream::sentry s{os};
    if (!s)
    {
      return os;
    }
    os << delim;
    for (auto it = bitmask.getBegin(); it != bitmask.getEnd(); ++it)
    {
      os << it->first << static_cast< char >(it->second);
    }
    return os << delim;
  }

  bool equals(const malakhov::Haffman::BitmaskT& op1, const malakhov::Haffman::BitmaskT& op2)
  {
    if (op1.getSize() != op2.getSize())
    {
      return false;
    }
    auto it1 = op1.getBegin();
    for (auto it2 = op2.getBegin(); it2 != op2.getEnd(); ++it2)
    {
      if (*it1 != *it2)
      {
        return false;
      }
    }
    return true;
  }
}

malakhov::Haffman::Haffman(const BitmaskT& bitmask, const std::string& encoded):
  encoded_(encoded),
  bitmask_(bitmask)
{}

malakhov::Haffman malakhov::Haffman::produce(const std::string& decoded)
{
  FrequencyT frequence = getFrequency(decoded);
  BitmaskT bitmask;
  {
    HaffmanTree haffman = HaffmanTree::construct(frequence);
    bitmask = haffman.getBitMask();
  }
  const unsigned char bitsPerEncodedLetter = getBitsPerEncodedLetter(bitmask);
  char bitsUntilFlush = BYTE_SIZE;
  std::string encoded;
  size_t encodedBits = 0;
  for (unsigned char&& letter: decoded)
  {
    encodedBits <<= bitsPerEncodedLetter;
    encodedBits += bitmask[letter];
    bitsUntilFlush -= bitsPerEncodedLetter;
    while (bitsUntilFlush <= 0)
    {
      flush(encodedBits, -bitsUntilFlush, encoded);
      bitsUntilFlush += BYTE_SIZE;
    }
  }
  encodedBits <<= bitsPerEncodedLetter;
  encodedBits += bitmask['\0'];
  bitsUntilFlush -= bitsPerEncodedLetter;
  if (bitsUntilFlush < 0)
  {
    flush(encodedBits, -bitsUntilFlush, encoded);
    bitsUntilFlush += BYTE_SIZE;
  }
  if (bitsUntilFlush > 0)
  {
    flush(encodedBits << bitsUntilFlush, 0, encoded);
  }
  return Haffman{bitmask, encoded};
}

std::string malakhov::Haffman::decode() const
{
  std::string decoded;
  const BitmaskT bitmask = getReversedMask(bitmask_);
  const unsigned char bitsPerEncodedLetter = getBitsPerEncodedLetter(bitmask);
  size_t encodedBits = 0;
  unsigned char bitsCount = 0;
  bool endSymbolSpoted = false;
  for (auto it = encoded_.begin(); !endSymbolSpoted; ++it)
  {
    const unsigned char letter = *it;
    bitsCount += BYTE_SIZE;
    encodedBits <<= BYTE_SIZE;
    encodedBits += letter;
    while (bitsCount >= bitsPerEncodedLetter)
    {
      bitsCount -= bitsPerEncodedLetter;
      const unsigned char encodedLetter = getBits(encodedBits, bitsPerEncodedLetter, bitsCount);
      const unsigned char decodedLetter = bitmask[encodedLetter];
      if (!decodedLetter)
      {
        endSymbolSpoted = true;
        break;
      }
      decoded.push_back(decodedLetter);
    }
  }
  return decoded;
}

const std::string& malakhov::Haffman::getEncoded() const noexcept
{
  return encoded_;
}

const BitmaskT& malakhov::Haffman::getBitmask() const noexcept
{
  return bitmask_;
}

std::ostream& malakhov::operator<<(std::ostream& os, const Haffman& haffman)
{
  std::ostream::sentry s{os};
  if (!s)
  {
    return os;
  }
  return print(os, haffman.getBitmask()) << ' ' << haffman.getEncoded();
}

bool malakhov::operator==(const Haffman& op1, const Haffman& op2)
{
  return equals(op1.getBitmask(), op2.getBitmask()) && op1.getEncoded() == op2.getEncoded();
}
