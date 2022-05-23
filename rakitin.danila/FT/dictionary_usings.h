#ifndef DICTIONARY_USINGS_HPP
#define DICTIONARY_USINGS_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

namespace rakitin {
  using dictionary = std::map< std::string, std::set< int > >;
  using word_refs = dictionary::value_type;
  using dictionaries = std::map< std::string, dictionary >;
  using ref_finder = std::pair< word_refs, bool >;
}

#endif
