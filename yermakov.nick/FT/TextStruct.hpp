#ifndef TEXTSTRUCT_HPP
#define TEXTSTRUCT_HPP

#include <string>
#include <iosfwd>
#include <map>

#include "HuffmanTree.hpp"
#include "CharDataStruct.hpp"

namespace yermakov
{
  struct Text
  {
    Text();
    Text(const CharData&, const HuffmanTree&);
    CharData data_;
    HuffmanTree tree_;
    bool isCompress_;
  };

  Text compress(const Text&);
  Text decompress(const Text&);
  std::istream& operator>>(std::istream&, Text&);
  std::ostream& operator<<(std::ostream&, const Text&);
}

#endif
