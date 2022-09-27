#include "Utilities.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <fstream>
#include "userCommands.hpp"

namespace shkroba
{
  using namespace std::placeholders;
  std::ostream& operator<<(std::ostream& out, std::set< std::string >& set)
  {
    for (auto& item: set)
    {
      out << item << ' ';
    }
    out << '\n';
    return out;
  }

  bool isHere(const Dictionary& current, const pairER& pair, std::set< std::string >& set)
  {
    auto temp = current.search(pair.first);
    if (temp != current.end())
    {
      auto begin1 = set.begin();
      auto end1 = set.end();
      auto begin2 = temp->second->begin();
      auto end2 = temp->second->end();
      std::set_intersection(begin1, end1, begin2, end2, std::inserter(set, set.begin()));
      return true;
    }
    return false;
  }

  Dictionary createCommonDictionary(std::vector< Dictionary >& common)
  {
    Dictionary result("result");
    Dictionary dictionary = *common.begin();
    for (const auto& pair: dictionary)
    {
      std::set< std::string > set = *pair.second;
      auto number = std::count_if(common.begin(), common.end(), std::bind(isHere, _1, pair, set));
      if (number == (common.size()))
      {
        setPointer ptr = std::make_shared< std::set< std::string > >(set);
        pairER newItem = std::make_pair(pair.first, ptr);
        result.insert(newItem);
      }
    }
    return result;
  }

  void doPrintDictionary(const Dictionary& dictionary, std::ostream& out)
  {
    dictionary.printDictionary(out);
  }

  void doSize(const Dictionary& dictionary, std::ostream& out)
  {
    out << dictionary.size();
  }

  void doFindWord(const Dictionary& dictionary, std::string letter, std::ostream& out)
  {
    dictionary.findWord(letter, std::cout);
  }

  std::string nextWord(std::string& str)
  {
    std::string word = str.substr(0, str.find_first_of(' '));
    if (str.find_first_of(' ') == -1)
    {
      if (word[word.size() - 1] == '\r')
      {
        word = word.substr(0, word.size() - 1);
      }
      str.clear();
    }
    else
    {
      str.erase(0, str.find_first_of(' ') + 1);
    }
    return word;
  }

  std::vector< Dictionary > createDictionariesFromFile(std::istream& in)
  {
    std::vector< Dictionary > resultVector;
    while (!in.eof())
    {
      Dictionary dictionary;
      in >> dictionary;
      if (!dictionary.getName().empty())
      {
        resultVector.push_back(dictionary);
      }
    }
    return resultVector;
  }

  void doPrintInFile(const Dictionary& dictionary, const std::string& fileName)
  {
    std::ofstream fout(fileName);
    if (!fout.is_open())
    {
      throw std::invalid_argument("File is not open\n");
    }
    if (fout.is_open())
    {
      dictionary.printDictionary(fout);
    }
  }
}
