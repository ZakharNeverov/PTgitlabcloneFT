#ifndef COMMANDS_SYSTEM_HPP
#define COMMANDS_SYSTEM_HPP

#include "dicts.hpp"

namespace bokov
{
  void printDict(std::string&, dicts_list&, std::ostream&);
  void loadDict(std::string&, dicts_list&);
  void removeElement(std::string&, dicts_list&);
  void mergeDicts(std::string&, dicts_list&);
  void intersectDicts(std::string&, dicts_list&);
  void complementDicts(std::string&, dicts_list&);
  void getTop(std::string&, dicts_list&, std::ostream&);
  void insertInDict(std::string&, dicts_list&);
  void searchInDict(std::string&, dicts_list&, std::ostream&);
  void deleteDict(std::string&, dicts_list&);
}
#endif
