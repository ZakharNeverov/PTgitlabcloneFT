#ifndef ONE_TYPED_COMMAND_HPP
#define ONE_TYPED_COMMAND_HPP

#include "DictionaryAliases.hpp"

namespace maistrenko
{
  void createDict(std::string&, std::ostream&, Dicts&);
  void getDict(std::string&, std::istream&, std::ostream&, Dicts&);
  void getTranslations(std::string&, std::ostream&, const Dicts&);
  void outputExactDict(std::string&, std::ostream&, const Dicts&);
  void addNewWord(std::string&, std::ostream&, Dicts&);
  void deleteDict(std::string&, std::ostream&, Dicts&);
  void deleteWord(std::string&, std::ostream&, Dicts&);
  void deleteTranslation(std::string&, std::ostream&, Dicts&);
  void unionDictionaries(std::string&, std::ostream&, Dicts&);
  void intersectDictionaries(std::string&, std::ostream&, Dicts&);
  void substractDictionaries(std::string&, std::ostream&, Dicts&);
  void loadDict(std::string&, std::ostream&, Dicts&);
  void saveDict(std::string&, std::ostream&, const Dicts&);
}

#endif
