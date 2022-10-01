#ifndef CHARDATASTRUCT_HPP
#define CHARDATASTRUCT_HPP

#include <string>
#include <iosfwd>
#include <map>
#include <cstddef>

namespace yermakov
{
  using FreqMap = std::map< char, std::size_t >;

  struct CharData
  {
    CharData();
    void calculateFrequency();
    std::string language_;
    std::string text_;
    FreqMap freqDict_;
  };

  std::istream& operator>>(std::istream&, CharData&);
}

#endif
