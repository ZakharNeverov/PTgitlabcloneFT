#ifndef DICTIONARY_USINGS_HPP
#define DICTIONARY_USINGS_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

namespace rakitin
{
  using word_refs = std::pair< std::string, std::set< int > >;
  using dictionary = std::vector< word_refs >;
  using dictionaries = std::map< std::string, dictionary >;
  using ref_finder = std::pair< word_refs, bool >;

  bool operator<(const word_refs& ref1, const word_refs& ref2);

}

#endif
