#include "commandWrappers.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include "iofmtguard.h"

namespace
{
  std::string NOT_ENOUGH_ARGS = "Not enough args";
  using namespace dudnik;
  struct wrapper
  {
    void operator()(
      Args& words,
      AllDicts& dicts,
      Dict (*BinOp)(const Args&, const AllDicts&)
    )
    {
      if (words.size() < 3)
      {
        throw std::invalid_argument(NOT_ENOUGH_ARGS);
      }
      std::string name = words.front();
      words.pop_front();
      dicts.emplace(name, BinOp(words, dicts));
    }
  };
  using Keys = std::list< int >;
  Keys getKeys(const Args& words)
  {
    Keys keys;
    for(auto&& word: words)
    {
      keys.push_back(std::stoi(word));
    }
    return keys;
  }
}

namespace dudnik
{
  void printWrapper(Args& words, AllDicts& dicts)
  {
    if (words.empty())
    {
      throw std::invalid_argument(NOT_ENOUGH_ARGS);
    }
    if (words.front() == "all")
    {
      printAll(std::cout, dicts);
      return;
    }
    print(std::cout, words, dicts);
  }
  void complementWrapper(Args& words, AllDicts& dicts)
  {
    wrapper wrap;
    wrap(words, dicts, complement);
  }
  void intersectWrapper(Args& words, AllDicts& dicts)
  {
    wrapper wrap;
    wrap(words, dicts, intersect);
  }
  void uniteWrapper(Args& words, AllDicts& dicts)
  {
    if ((words.size() == 2) && (words.back() == "all"))
    {
      dicts.emplace(words.front(), uniteAll(dicts));
      return;
    }
    wrapper wrap;
    wrap(words, dicts, unite);
  }
  void removeWrapper(Args& words, AllDicts& dicts)
  {
    if (words.empty())
    {
      throw std::invalid_argument(NOT_ENOUGH_ARGS);
    }
    if ((words.size() == 1) && (words.front() == "all"))
    {
      dicts.clear();
      return;
    }
    if ((words.size() >= 2) && (std::isdigit(words.back().front())))
    {
      std::string name = words.front();
      words.pop_front();
      auto keys = getKeys(words);
      remove(name, keys, dicts);
    }
    else
    {
      remove(words, dicts);
    }
  }
  void replaceWrapper(Args& words, AllDicts& dicts)
  {
    if (words.size() != 3)
    {
      throw std::invalid_argument("Bad number of args");
    }
    std::string name = words.front();
    int key = std::stoi(*(++words.cbegin()));
    std::string newValue = words.back();
    replace(name, key, newValue, dicts);
  }
  void equalWrapper(Args& words, AllDicts& dicts)
  {
    if (words.size() < 2)
    {
      throw std::invalid_argument(NOT_ENOUGH_ARGS);
    }
    iofmtguard guard(std::cout);
    std::cout << std::boolalpha << equal(words, dicts) << std::endl;
  }
  void insertWrapper(Args& words, AllDicts& dicts)
  {
    if (words.size() != 3)
    {
      throw std::invalid_argument("Bad number of args");
    }
    std::string name = words.front();
    int key = std::stoi(*(++words.cbegin()));
    std::string value = words.back();
    insert(name, key, value, dicts);
  }
  void addWrapper(Args& words, AllDicts& dicts)
  {
    if (words.empty())
    {
      throw std::invalid_argument(NOT_ENOUGH_ARGS);
    }
    for (auto&& name: words)
    {
      dicts.emplace(name, Dict());
    }
  }
  void clearWrapper(Args& words, AllDicts& dicts)
  {
    if (words.empty())
    {
      throw std::invalid_argument(NOT_ENOUGH_ARGS);
    }
    if (words.front() == "all")
    {
      clearAll(dicts);
    }
    else
    {
      clear(words, dicts);
    }
  }
}
