#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <map>
#include <string>
#include <functional>
#include <iostream>

namespace bokiy
{
  using node_t = std::pair< const std::string, size_t >;
  using map_t = std::map<std::string, size_t>;
  using maps_t = std::map< std::string, map_t >;
  using get_el_f_t_ = std::function< node_t& (map_t&) >;
  using comp_f_t = std::function< bool(node_t&, map_t&) >;

  void make(std::istream&, maps_t&);
  void write(std::istream&, maps_t&);
  void read(std::istream&, maps_t&);
  void makeFrequentOrRarest(std::istream&, maps_t&, get_el_f_t_);
  void complementOrIntersect(std::istream&, maps_t&, comp_f_t);
  void print(std::istream&, maps_t&);
  void printFrequentOrRarestWord(std::istream&, maps_t&, get_el_f_t_);
  

  bool frequentCompare(node_t& first, node_t& second);
  node_t& getFrequentElement(map_t&);
  node_t& getRarestElement(map_t&);
  bool isNotSame(node_t&, map_t&);
  bool isSame(node_t&, map_t&);
}

#endif
