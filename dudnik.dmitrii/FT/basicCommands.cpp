#include "basicCommands.h"
#include <iostream>
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
    auto returnable = lhs;
    auto rhsIt = rhs.cbegin();
    auto retIt = returnable.begin();
    std::less< int > cmp;
    while ((retIt != returnable.end()) && (rhsIt != rhs.cend()))
    {
      if (cmp(retIt->first, rhsIt->first))
      {
        while((retIt != returnable.end()) && cmp(retIt->first, rhsIt->first))
        {
          ++retIt;
        }
      }
      else if (cmp(rhsIt->first, retIt->first))
      {
        while ((rhsIt != rhs.cend()) && cmp(rhsIt->first, retIt->first))
        {
          ++rhsIt;
        }
      }
      if ((retIt == returnable.end()) || (rhsIt == rhs.cend()))
      {
        return returnable;
      }
      if (!cmp(retIt->first, rhsIt->first) && !cmp(rhsIt->first, retIt->first))
      {
        ++retIt;
        returnable.erase(rhsIt->first);
        ++rhsIt;
      }
      else
      {
        ++retIt;
        ++rhsIt;
      }
    }
    return returnable;
  }
  Dict basicIntersect(const Dict& lhs, const Dict& rhs)
  {
    auto returnable = Dict();
    auto lhsIt = lhs.cbegin();
    auto rhsIt = rhs.cbegin();
    std::less< int > cmp;
    while ((lhsIt != lhs.cend()) && (rhsIt != rhs.cend()))
    {
      if (cmp(lhsIt->first, rhsIt->first))
      {
        while ((lhsIt != lhs.cend()) && cmp(lhsIt->first, rhsIt->first))
        {
          ++lhsIt;
        }
      }
      else if (cmp(rhsIt->first, lhsIt->first))
      {
        while ((rhsIt != rhs.cend()) && cmp(rhsIt->first, lhsIt->first))
        {
          ++rhsIt;
        }
      }
      if ((lhsIt == lhs.cend()) || (rhsIt == rhs.cend()))
      {
        return returnable;
      }
      if (!cmp(lhsIt->first, rhsIt->first) && !cmp(rhsIt->first, lhsIt->first))
      {
        returnable.emplace(lhsIt->first, lhsIt->second);
      }
      ++lhsIt;
      ++rhsIt;
    }
    return returnable;
  }
  Dict basicUnite(const Dict& lhs, const Dict& rhs)
  {
    if (lhs.empty())
    {
      return rhs;
    }
    if (lhs.empty())
    {
      return lhs;
    }
    auto returnable = lhs;
    auto rhsIt = rhs.cbegin();
    while (rhsIt != rhs.cend())
    {
      if (returnable.find(rhsIt->first) == returnable.cend())
      {
        returnable.emplace(rhsIt->first, rhsIt->second);
      }
      ++rhsIt;
    }
    return returnable;
  }
}
