#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <functional>
#include <map>

namespace varnavskii
{
  using dict_t = std::map< char, std::string >;
  using texts_t = std::map< std::string, std::string >;
  using dicts_t = std::map< std::string, varnavskii::dict_t >;
  void add(texts_t&, dicts_t&, std::istream&);
  void code(texts_t&, dicts_t&, std::istream&);
  void encode(texts_t&, dicts_t&, std::istream&);
  void decode(texts_t&, dicts_t&, std::istream&);
  void frequency(texts_t&, std::istream&, std::ostream&);
  void print(texts_t&, dicts_t&, std::istream&, std::ostream&);
  void merge(texts_t&, dicts_t&, std::istream&);
}

#endif
