#ifndef COMMAND_SYSTEM_HPP
#define COMMAND_SYSTEM_HPP

#include <ios>
#include <unordered_map>
#include <string>
#include <set>
#include <vector>

namespace stretenskiy
{
  namespace function
  {
    using myDict = std::unordered_map< std::string, std::set < std::string > >;
    using nameDict = std::vector < std::string >;
    using vecDict = std::vector< myDict >;

    void creationDict(std::ostream &, vecDict &, nameDict &, std::istream &);
    void add(std::ostream &, vecDict &, const nameDict &, std::istream &);
    void removeWord(std::ostream &, vecDict &, const nameDict &, std::istream &);
    void search(std::ostream &, const vecDict &, const nameDict &, std::istream &);
    void clearDict(std::ostream &, vecDict &, const nameDict &, std::istream &);
    void unionDict(std::ostream &, vecDict &, const nameDict &, std::istream &);
    void intersectDict(std::ostream &, vecDict &, const nameDict &, std::istream &);
    void print(std::ostream &, const vecDict &, const nameDict &, std::istream &);
  }
}

#endif
