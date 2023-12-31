#include "Commands.h"
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <iterator>
#include "basicCommands.h"
#include "printMessages.h"

namespace
{
  std::string BAD_ARGS = "Bad args";
  using namespace dudnik;
  struct applyToRange
  {
      dudnik::Dict operator()(
      const Args& args,
      const AllDicts& dicts,
      Dict (*operation)(const Dict& lhs, const Dict& rhs)
    )
    {
      if ((args.size() < 2) || (dicts.size() < 2))
      {
        throw std::invalid_argument(BAD_ARGS);
      }
      auto firstIt = dicts.find(args.front());
      if (firstIt == dicts.end())
      {
        throw std::invalid_argument(BAD_ARGS);
      }
      auto prev = firstIt->second;
      return std::accumulate(
        ++std::begin(args),
        std::end(args),
        prev,
        [&](dudnik::Dict& curr, const std::string& name) -> dudnik::Dict
        {
          auto nextIt = dicts.find(name);
          if (nextIt == dicts.end())
          {
            throw std::invalid_argument(BAD_ARGS);
          }
          return operation(curr, nextIt->second);
        }
      );
    }
  };
}

namespace dudnik
{
  std::ostream& print(std::ostream& out, const Args& args, const AllDicts& dicts)
  {
    for (auto&& name: args)
    {
      auto dictIt = dicts.find(name);
      if (dictIt == dicts.end())
      {
        throw std::invalid_argument(BAD_ARGS);
      }
      basicPrint(out, name, dictIt->second);
    }
    return out;
  }
  std::ostream& printAll(std::ostream& out, const AllDicts& dicts)
  {
    if (dicts.empty())
    {
      printEmpty(out);
      return out;
    }
    for (auto&& elem: dicts)
    {
      basicPrint(out, elem.first, elem.second);
    }
    return out;
  }
  Dict complement(const Args& args, const AllDicts& dicts)
  {
    applyToRange apply;
    return apply(args, dicts, basicComplement);
  }
  Dict intersect(const Args& args, const AllDicts& dicts)
  {
    applyToRange apply;
    return apply(args, dicts, basicIntersect);
  }
  Dict unite(const Args& args, const AllDicts& dicts)
  {
    applyToRange apply;
    return apply(args, dicts, basicUnite);
  }
  Dict uniteAll(const AllDicts& dicts)
  {
    if (dicts.size() < 2)
    {
      throw std::invalid_argument(BAD_ARGS);
    }
    auto prev = dicts.begin()->second;
    return std::accumulate(
      ++std::begin(dicts),
      std::end(dicts),
      prev,
      [&](Dict& curr, const std::pair< std::string, Dict >& next) -> Dict
      {
        return basicUnite(curr, next.second);
      }
    );
  }
  void replace(const std::string& name, int key, const std::string& newValue, AllDicts& dicts)
  {
    auto dictIt = dicts.find(name);
    if (dictIt == dicts.end())
    {
      throw std::invalid_argument(BAD_ARGS);
    }
    auto nodeIt = dictIt->second.find(key);
    if (nodeIt != dictIt->second.end())
    {
      nodeIt->second = newValue;
    }
  }
  void remove(const std::string& name, const Keys& keys, AllDicts& dicts)
  {
    auto dictIt = dicts.find(name);
    if (dictIt == dicts.end())
    {
      throw std::invalid_argument(BAD_ARGS);
    }
    for (auto&& key: keys)
    {
      dictIt->second.erase(key);
    }
  }
  void remove(const Args& names, AllDicts& dicts)
  {
    for (auto&& name: names)
    {
      dicts.erase(name);
    }
  }
  bool equal(const Args& args, const AllDicts& dicts)
  {
    if (args.size() < 2)
    {
      throw std::invalid_argument(BAD_ARGS);
    }
    auto sampleIt = dicts.find(args.front());
    if (sampleIt == dicts.end())
    {
      throw std::invalid_argument(BAD_ARGS);
    }
    auto diffElem = std::find_if_not(
      std::begin(args),
      std::end(args),
      [&](const std::string& name) -> bool
      {
        auto dictIt = dicts.find(name);
        if (dictIt == dicts.end())
        {
          throw std::invalid_argument(BAD_ARGS);
        }
        return (dictIt->second == sampleIt->second);
      }
    );
    return (diffElem == args.end());
  }
  void insert(const std::string& name, int key, const std::string& value, AllDicts& dicts)
  {
    auto dictIt = dicts.find(name);
    if (dictIt == dicts.end())
    {
      throw std::invalid_argument(BAD_ARGS);
    }
    dictIt->second.emplace(key, value);
  }
  void clear(const Args& names, AllDicts& dicts)
  {
    for (auto&& name: names)
    {
      auto it = dicts.find(name);
      if (it == dicts.end())
      {
        throw std::invalid_argument(BAD_ARGS);
      }
      it->second.clear();
    }
  }
  void clearAll(AllDicts& dicts)
  {
    for (auto&& dict: dicts)
    {
      dict.second.clear();
    }
  }
}
