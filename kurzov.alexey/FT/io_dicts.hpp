#ifndef IO_DICTS_HPP
#define IO_DICTS_HPP

#include <iosfwd>
#include <string>
#include <set>
#include <map>

namespace kurzov
{
  using en_word_t = std::string;
  using ru_list_t = std::set< std::string >;
  using dict_t = std::map< en_word_t, ru_list_t >;
  using dicts_t = std::map< std::string, dict_t >;

  dict_t readDictFromStream(std::istream&);
  ru_list_t splitStringToTranslation(std::string);
  void printDict(const dict_t&, std::ostream&, const char* delimeter = ",", bool with_new_line = true);
  void printRuList(const ru_list_t&, std::ostream&, const char* delimeter = ",", bool with_new_line = true);

  bool loadNewDict(std::string, std::string, dicts_t&);
}

#endif
