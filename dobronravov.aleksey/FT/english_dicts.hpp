#ifndef ENGLISH_DICTS_HPP_FT
#define ENGLISH_DICTS_HPP_FT

#include <map>
#include <deque>
#include <string>
#include <iosfwd>

namespace dobronravov
{
  using word_t = std::string;
  using ru_list_t = std::deque< word_t >;
  using dict_t = std::map< std::string, ru_list_t >;
  using dicts_t = std::map< std::string, dict_t >;

  dict_t readDictFromStream(std::istream&);
  ru_list_t readRuList(std::string);

  void outDict(const dict_t&, std::ostream&);
  void outTranslation(const dict_t::value_type&, std::ostream& out);
  void outRuList(const ru_list_t&, std::ostream&);

  dict_t unionDicts(const dict_t&, const dict_t&);
  ru_list_t unionLists(const ru_list_t&, const ru_list_t&);

  dict_t intersectDicts(const dict_t&, const dict_t&);
  ru_list_t intersectLists(const ru_list_t&, const ru_list_t&);

  dict_t complementDicts(const dict_t&, const dict_t&);
  ru_list_t complementLists(const ru_list_t&, const ru_list_t&);
}

#endif
