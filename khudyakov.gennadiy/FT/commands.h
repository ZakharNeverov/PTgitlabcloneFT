#ifndef COMMANDS_H
#define COMMANDS_H
#include <iosfwd>
#include <string>
#include <list>
#include <map>

namespace khudyakov
{
  using List_t = std::list< size_t >;
  using Dict_t = std::map< std::string, List_t >;
  using Dicts_t = std::map< std::string, Dict_t >;

  std::string getWord(std::string&);
  void parseStringToDict(std::string, size_t, Dict_t&);

  std::ostream& doPrint(const Dict_t&, std::ostream&);
  double doSimilarity(const Dict_t&, const Dict_t&);

  void print(std::string, const Dicts_t&);
  void similarity(std::string, const Dicts_t&);
  void deleteOften(std::string, Dicts_t&);
  void deleteRare(std::string, Dicts_t&);
  void deleteWord(std::string, Dicts_t&);
  void deleteWordsInString(std::string, Dicts_t&);
  void printWordsInString(std::string, const Dicts_t&);
  void rename(std::string, Dicts_t&);
  void complement(std::string, Dicts_t&);
  void intersect(std::string, Dicts_t&);
  void unit(std::string, Dicts_t&);
}

#endif
