#ifndef DICTIONARY_INTERACTION_HPP
#define DICTIONARY_INTERACTION_HPP
#include <string>
#include <vector>
#include "Dictionary.hpp"
namespace akimov
{
  using cnstStr = const std::string;
  using cnstDict = const akimov::Dictionary;
  using vectorD = std::vector< akimov::Dictionary >;
  using vectorCIt = akimov::vectorD::const_iterator;

  struct isName
  {
    bool operator()(cnstDict& dictionary, cnstStr& name)
    {
      return dictionary.getName() == name;
    }
  };

  void isAvailable(const vectorD& dicts, cnstStr& name);
  auto isFound(vectorD& dicts, cnstStr& name);
  auto isFound(const vectorD& dicts, cnstStr& name);
  void getNamesAndDoCommand(std::istream& input, vectorD& dicts, cnstStr& cmd);
  void intersectionCommand(vectorD& dicts, cnstStr& name, vectorCIt it1, vectorCIt it2, cnstStr& cmd, char c);

  void create(std::istream& input, vectorD& dicts);
  void print(std::istream& input, std::ostream& output, const vectorD& dicts);
  void getMostPopular(std::istream& input, std::ostream& output, const vectorD& dicts);
  void find(std::istream& input, std::ostream& output, const vectorD& dicts);
  void drop(std::istream& input, vectorD& dicts);
  void add(std::istream& input, vectorD& dicts);
  void remove(std::istream& input, vectorD& dicts);
  void intersection(std::istream& input, vectorD& dicts);
  void intersectionFromLetter(std::istream& input, vectorD& dicts);
}
#endif
