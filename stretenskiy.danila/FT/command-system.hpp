#ifndef COMMAND_SYSTEM_HPP
#define COMMAND_SYSTEM_HPP

#include <ios>
#include <vector>
#include "dictionary.hpp"

namespace stretenskiy
{
  namespace function
  {
    using vecDict = std::vector< Dictionary >;

    void creationDict(std::ostream &, vecDict &, std::istream &);
    void add(std::ostream &, vecDict &, std::istream &);
    void removeWord(std::ostream &, vecDict &, std::istream &);
    void search(std::ostream &, const vecDict &, std::istream &);
    void clearDict(std::ostream &, vecDict &, std::istream &);
    void unionDict(std::ostream &, vecDict &, std::istream &);
    void intersectDict(std::ostream &, vecDict &, std::istream &);
    void print(std::ostream &, const vecDict &, std::istream &);
  }
}

#endif
