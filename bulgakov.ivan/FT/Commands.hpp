#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <map>
#include <string>
#include <functional>
#include <iostream>

namespace bulgakov
{
  using node_t = std::pair< const std::string, size_t >;
  using map_t = std::map<std::string, size_t>;
  using maps_t = std::map< std::string, map_t >;
  using get_el = std::function< node_t& (map_t&) >;
  using cmp_func = std::function< bool(node_t&, map_t&) >;

  void analyze(std::istream&, maps_t&);
  void remove(std::istream&, maps_t&);
  void mergeDir(std::istream&, maps_t&);
  void doUniqDict(std::istream&, maps_t&, cmp_func);
  void print(std::istream&, maps_t&, std::ostream&);
  void doFreqDictRare(std::istream&, maps_t&, get_el);
  void searchWord(std::istream&, maps_t&, std::ostream&);

  bool frequentCompare(node_t& first, node_t& second);
  node_t& getFreqEl(map_t&);
  node_t& getRareEl(map_t&);
  bool isNotSame(node_t&, map_t&);
  bool isSame(node_t&, map_t&);
}

#endif
