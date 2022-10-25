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

  void doInter(dict_t&, std::string, std::string, std::string);
  void doCompl(dict_t&, std::string, std::string, std::string);
  std::string getWord(std::string&);

  bool hasPrefix(const std::string&, const std::string&);
  bool isUniqueName(std::string, const dict_t&);
}
#endif
