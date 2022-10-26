#ifndef AUXILIARY_METHODS_HPP
#define AUXILIARY_METHODS_HPP

#include <string>
#include <set>
#include <map>

namespace gavrikov
{
  using dictName = std::string;
  using enWords = std::set< std::string >;
  using dict_t = std::map< dictName, enWords >;
  using cIterDict = dict_t::const_iterator;

  using constStr = const std::string;
  void doInter(dict_t&, constStr&, constStr&, constStr&);
  void doCompl(dict_t&, constStr&, constStr&, constStr&);
  void doCycle(cIterDict&, cIterDict&, enWords&, bool);
  std::string getWord(std::string&);

  bool hasPrefix(const std::string&, const std::string&);
  bool isUniqueName(constStr&, const dict_t&);
}
#endif
