#ifndef COMMANDS_WITH_DICT_HPP
#define COMMANDS_WITH_DICT_HPP

#include <iostream>
#include <set>
#include <map>
#include <string>

namespace gavrikov
{
  using dictName = std::string;
  using enWords = std::set< std::string >;
  using dict_t = std::map< dictName, enWords >;

  void createDict(dict_t&, std::istream&);
  void printDict(const dict_t&, std::istream&, std::ostream&);
  void insertWord(dict_t&, std::istream&);
  void intersection(dict_t&, std::istream&);
  void complement(dict_t&, std::istream&);
  void countPref(const dict_t&, std::istream&, std::ostream&);
  void dropDict(dict_t&, std::istream&);
  void unload(dict_t&, std::istream&);
  void load(dict_t&, std::istream&);
}
#endif
