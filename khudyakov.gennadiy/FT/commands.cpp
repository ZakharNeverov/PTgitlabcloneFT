#include "commands.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>
#include "StreamGuard.h"
#include "MessagesIO.h"

namespace
{
  using Pair_t = std::pair< std::string, khudyakov::List_t >;
  bool compareSizeList(const Pair_t& lhs, const Pair_t& rhs)
  {
    return lhs.second.size() < rhs.second.size();
  }

  bool wordEqual(const Pair_t& lhs, const Pair_t& rhs)
  {
    return lhs.first == rhs.first;
  }

  bool compareWord(const Pair_t& lhs, const Pair_t& rhs)
  {
    return lhs.first < rhs.first;
  }

  bool compareSizeWord(const Pair_t& lhs, const Pair_t& rhs)
  {
    return lhs.first.size() < rhs.first.size();
  }

  size_t sumSimilarityWords(size_t result, const Pair_t& exp, const khudyakov::Dict_t& dict)
  {
    using namespace std::placeholders;
    auto f = std::bind(wordEqual, _1, exp);
    if (std::find_if(dict.cbegin(), dict.cend(), f) != dict.cend())
    {
      result += 2;
    }
    return result;
  }

  bool wordInString(const size_t number, const Pair_t& x)
  {
    return std::find(x.second.cbegin(), x.second.cend(), number) != x.second.cend();
  }

  void redactWord(std::string& str)
  {
    if (std::ispunct(str.back()))
    {
      str.pop_back();
      redactWord(str);
    }
  }

  template < typename Cmp >
  void doDeleteOften(std::string& str, khudyakov::Dicts_t& dicts, Cmp cmp)
  {
    std::string dictName = "";
    while (!str.empty())
    {
      dictName = khudyakov::getWord(str);
      if (dicts.find(dictName) == dicts.end() || (*dicts.find(dictName)).second.empty())
      {
        throw std::logic_error{"Invalid command"};
      }
      auto iterDict = dicts.find(dictName);
      auto i = std::max_element((*iterDict).second.cbegin(), (*iterDict).second.cend(), cmp);
      (*iterDict).second.erase(i);
    }
  }
}

namespace khudyakov
{
  std::string getWord(std::string& str)
  {
    while (std::isspace(str[0]) && !str.empty())
    {
      str.erase(0, 1);
    }
    if (str.empty())
    {
      throw std::logic_error{"String is empty"};
    }
    auto pos = str.find(' ');
    std::string word = "";
    if (pos == str.npos)
    {
      word = str;
      str.clear();
      return word;
    }
    word = str.substr(0, pos);
    str.erase(0, pos + 1);
    return word;
  }

  void parseStringToDict(std::string str, size_t number, Dict_t& dict)
  {
    std::string word = "";
    while (!str.empty())
    {
      word = khudyakov::getWord(str);
      std::transform(word.begin(), word.end(), word.begin(), std::tolower);
      redactWord(word);
      if (dict.find(word) != dict.end())
      {
        (*dict.find(word)).second.push_back(number);
        (*dict.find(word)).second.unique();
      }
      else
      {
        dict.emplace(word, List_t());
        (*dict.find(word)).second.push_back(number);
      }
    }
  }

  std::ostream& doPrint(const Dict_t& dict, std::ostream& stream)
  {
    using ostreamIterator_t = typename std::ostream_iterator< size_t >;
    if (dict.empty())
    {
      khudyakov::emptyMessage(stream);
    }
    khudyakov::StreamGuard guard(stream);
    for (auto i = dict.cbegin(); i != dict.cend(); i++)
    {
      stream << (*i).first;
      auto maxWordSize = (*std::max_element(dict.begin(), dict.end(), compareSizeWord)).first.size();
      stream << std::setw(maxWordSize - (*i).first.size() + 3);
      std::copy((*i).second.cbegin(), (*i).second.cend(), ostreamIterator_t(stream, " "));
      i != --dict.cend() ? stream << "\n" : stream << "";
    }
    return stream;
  }

  double doSimilarity(const Dict_t& dict1, const Dict_t& dict2)
  {
    size_t sum = dict1.size() + dict2.size();
    using namespace std::placeholders;
    size_t similarWords = std::accumulate(dict2.cbegin(), dict2.cend(), 0, std::bind(sumSimilarityWords, _1, _2, dict1));
    if (sum == 0)
    {
      return 0;
    }
    return (static_cast< double >(similarWords) / static_cast< double >(sum)) * 100;
  }

  void similarity(std::string str, const Dicts_t& dicts)
  {
    std::string dictName1 = khudyakov::getWord(str);
    std::string dictName2 = khudyakov::getWord(str);
    if (dicts.find(dictName1) == dicts.cend() || dicts.find(dictName2) == dicts.cend())
    {
      throw std::logic_error{"Invalid command"};
    }
    khudyakov::StreamGuard guard(std::cout);
    std::cout << std::fixed << std::setprecision(2);
    std::cout << doSimilarity((*dicts.find(dictName1)).second, (*dicts.find(dictName2)).second) << '%';
  }

  void print(std::string str, const Dicts_t& dicts)
  {
    auto dictName = khudyakov::getWord(str);
    if (dicts.find(dictName) == dicts.cend())
    {
      throw std::logic_error{"Invalid command"};
    }
    doPrint((*dicts.find(dictName)).second, std::cout);
  }

  void deleteOften(std::string str, Dicts_t& dicts)
  {
    doDeleteOften(str, dicts, compareSizeList);
  }

  void deleteRare(std::string str, Dicts_t& dicts)
  {
    using namespace std::placeholders;
    doDeleteOften(str, dicts, std::bind(compareSizeList, _2, _1));
  }

  void deleteWord(std::string str, Dicts_t& dicts)
  {
    std::string dictName = khudyakov::getWord(str);
    std::string word = "";
    while (!str.empty())
    {
      word = khudyakov::getWord(str);
      if (dicts.find(dictName) == dicts.end() || (*dicts.find(dictName)).second.empty())
      {
        throw std::logic_error{"Invalid command"};
      }
      (*dicts.find(dictName)).second.erase(word);
    }
  }

  void deleteWordsInString(std::string str, Dicts_t& dicts)
  {
    std::string dictName = khudyakov::getWord(str);
    std::string number = "";
    while (!str.empty())
    {
      number = khudyakov::getWord(str);
      auto iterDict = dicts.find(dictName);
      if (iterDict == dicts.end() || (*iterDict).second.empty())
      {
        throw std::logic_error{"Invalid command"};
      }
      using namespace std::placeholders;
      auto begin = (*iterDict).second.begin();
      auto end = (*iterDict).second.end();
      auto found = std::find_if(begin, end, std::bind(wordInString, std::stoi(number), _1));
      while (found != (*iterDict).second.end())
      {
        begin = (*iterDict).second.erase(found);
        found = std::find_if(begin, end, std::bind(wordInString, std::stoi(number), _1));
      }
    }
  }

  void printWordsInString(std::string str, const Dicts_t& dicts)
  {
    std::string dictName = khudyakov::getWord(str);
    std::string number = "";
    while (!str.empty())
    {
      number = khudyakov::getWord(str);
      auto iterDict = dicts.find(dictName);
      if (dicts.find(dictName) == dicts.end() || (*iterDict).second.empty())
      {
        throw std::logic_error{"Invalid command"};
      }
      std::list< std::string > list;
      auto start = (*iterDict).second.begin();
      using namespace std::placeholders;
      auto found = std::find_if(start, (*iterDict).second.end(), std::bind(wordInString, std::stoi(number), _1));
      while (found != (*iterDict).second.end())
      {
        list.push_back((*found).first);
        start = ++found;
        found = std::find_if(start, (*iterDict).second.end(), std::bind(wordInString, std::stoi(number), _1));
      }
      using ostreamIterator_t = typename std::ostream_iterator< std::string >;
      std::copy(list.cbegin(), list.cend(), ostreamIterator_t(std::cout, " "));
      if (!str.empty())
      {
        std::cout << '\n';
      }
    }
  }

  void rename(std::string str, Dicts_t& dicts)
  {
    std::string old_dict_name = "";
    std::string new_dict_name = "";
    while (!str.empty())
    {
      old_dict_name = khudyakov::getWord(str);
      new_dict_name = khudyakov::getWord(str);
      auto found = dicts.find(old_dict_name);
      if (found == dicts.end())
      {
        throw std::logic_error{"Invalid command"};
      }
      auto copy_data = (*found).second;
      dicts.erase(old_dict_name);
      dicts.emplace(new_dict_name, copy_data);
    }
  }

  void complement(std::string str, khudyakov::Dicts_t& dicts)
  {
    while (!str.empty())
    {
      std::string nameOfNewDict = khudyakov::getWord(str);
      std::string dictName1 = khudyakov::getWord(str);
      std::string dictName2 = khudyakov::getWord(str);
      auto iter1 = dicts.find(dictName1);
      auto iter2 = dicts.find(dictName2);
      if (iter1 == dicts.end() || iter2 == dicts.end())
      {
        throw std::logic_error{"Invalid command"};
      }
      auto itList1 = (*iter1).second;
      auto itList2 = (*iter2).second;
      khudyakov::Dict_t dict;
      std::set_difference(itList1.cbegin(), itList1.cend(), itList2.cbegin(),
        itList2.cend(), std::inserter(dict, dict.begin()), compareWord);
      dicts.emplace(nameOfNewDict, dict);
    }
  }

  void intersect(std::string str, khudyakov::Dicts_t& dicts)
  {
    std::string nameOfNewDict = khudyakov::getWord(str);
    std::string dictName1 = khudyakov::getWord(str);
    std::string dictName2 = khudyakov::getWord(str);
    auto iter1 = dicts.find(dictName1);
    auto iter2 = dicts.find(dictName2);
    if (iter1 == dicts.end() || iter2 == dicts.end())
    {
      throw std::logic_error{"Invalid command"};
    }
    auto itList1 = (*iter1).second;
    auto itList2 = (*iter2).second;
    khudyakov::Dict_t dict;
    std::set_intersection(itList1.cbegin(), itList1.cend(), itList2.cbegin(),
      itList2.cend(), std::inserter(dict, dict.begin()), compareWord);
    while (!str.empty())
    {
      dictName2 = khudyakov::getWord(str);
      iter2 = dicts.find(dictName2);
      if (iter2 == dicts.end())
      {
        throw std::logic_error{"Invalid command"};
      }
      itList2 = (*iter2).second;
      khudyakov::Dict_t temp;
      std::set_intersection(dict.cbegin(), dict.cend(), itList2.cbegin(),
        itList2.cend(), std::inserter(temp, temp.begin()), compareWord);
      dict = temp;
    }
    dicts.emplace(nameOfNewDict, dict);
  }

  void unit(std::string str, khudyakov::Dicts_t& dicts)
  {
    std::string nameOfNewDict = khudyakov::getWord(str);
    std::string dictName1 = khudyakov::getWord(str);
    auto iter1 = dicts.find(dictName1);
    auto itList1 = (*iter1).second;
    if (iter1 == dicts.end() || str.empty())
    {
      throw std::logic_error{"Invalid command"};
    }
    khudyakov::Dict_t dict;
    while (!str.empty())
    {
      std::string dictName2 = khudyakov::getWord(str);
      auto iter2 = dicts.find(dictName2);
      if (iter2 == dicts.end())
      {
        throw std::logic_error{"Invalid command"};
      }
      auto itList2 = (*iter2).second;
      std::set_union(itList1.cbegin(), itList1.cend(), itList2.cbegin(),
        itList2.cend(), std::inserter(dict, dict.begin()), compareWord);
    }
    dicts.emplace(nameOfNewDict, dict);
  }
}
