#include "basicCommands.h"
#include <iostream>
#include <iterator>
#include <algorithm>

namespace dudnik
{
  std::ostream& basicPrint(std::ostream& out, const std::string& name, const Dict& dict)
  {
    out << name;
    for (auto&& data: dict)
    {
      out << ' ' << data.first << ' ' << data.second;
    }
    out << '\n';
    return out;
  }
  Dict basicComplement(const Dict& lhs, const Dict& rhs)
  {
    Dict returnable;
    std::set_difference(
      std::begin(lhs), std::end(lhs),
      std::begin(rhs), std::end(rhs),
      std::inserter(returnable, std::begin(returnable))
    );
    return returnable;
  }
  Dict basicIntersect(const Dict& lhs, const Dict& rhs)
  {
    Dict returnable;
    std::set_intersection(
      std::begin(lhs), std::end(lhs),
      std::begin(rhs), std::end(rhs),
      std::inserter(returnable, std::begin(returnable))
    );
    return returnable;
  }
  Dict basicUnite(const Dict& lhs, const Dict& rhs)
  {
    Dict returnable;
    std::set_union(
      std::begin(lhs), std::end(lhs),
      std::begin(rhs), std::end(rhs),
      std::inserter(returnable, std::begin(returnable))
    );
    return returnable;
  }
}
