#ifndef COMMANDSWITHDICTSOFDICTS_H
#define COMMANDSWITHDICTSOFDICTS_H
#include <set>
#include <map>
#include <string>

namespace nikiforova {
  using Dict = std::set< std::string >;
  using DictOfDicts = std::map< std::string, Dict >;

  DictOfDicts readAllDictsFromStream(std::istream&);
  Dict convertStringToDict(std::string&);

  std::ostream& doPrint(std::ostream&, const std::string&, const Dict&);
  std::ostream& getWordsWithPrefix(std::ostream&, const std::string&, const Dict&);
  void doComplement(const std::string&, const std::string&, const std::string&, DictOfDicts&);
  void doIntersect(const std::string&, const std::string&, const std::string&, DictOfDicts&);
  void doUnion(const std::string&, const std::string&, const std::string&, DictOfDicts&);
  std::ostream& isContain(std::ostream&, const std::string&, const Dict&);
  void doSave(const std::string&, const std::string&, const Dict&);

  void print(std::string&, const DictOfDicts&);
  void complement(std::string&, DictOfDicts&);
  void intersect(std::string&, DictOfDicts&);
  void myUnion(std::string&, DictOfDicts&);
  void wordsWithPrefix(std::string&, const DictOfDicts&);
  void contain(std::string&, const DictOfDicts&);
  void save(std::string&, const DictOfDicts&);

}
#endif
