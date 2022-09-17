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
    Text() = default;
    Text(CharData, HuffmanTree);
    CharData data_;
    HuffmanTree tree_;
    bool isCompress_ = false;
  };

  Text compress(Text&);
  Text decompress(Text&);
  std::istream& operator>>(std::istream&, Text&);
  std::ostream& operator<<(std::ostream&, Text&);
}

#endif
