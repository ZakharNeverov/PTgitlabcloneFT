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
  using cRefStr = const std::string&;

  void doInter(dict_t&, cRefStr, cRefStr, cRefStr);
  void doCompl(dict_t&, cRefStr, cRefStr, cRefStr);
  void doInterCycle(cIterDict&, cIterDict&, enWords&);
  void doComplCycle(cIterDict&, cIterDict&, enWords&);
  void doCycle(cIterDict&, cIterDict&, enWords&);
  std::string getWord(std::string&);

  bool hasPrefix(cRefStr, cRefStr);
  bool isUniqueName(cRefStr, const dict_t&);
}
#endif
