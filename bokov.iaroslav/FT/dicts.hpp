#ifndef DICTS_HPP
#define DICTS_HPP

#include <map>
#include <set>
#include <string>

namespace bokov
{
  using dictionary = std::map< std::string, std::set< std::string > >;
  using dicts_list = std::map< std::string, dictionary >;
  void makeDictionary(const std::string&, dictionary&);
}
#endif