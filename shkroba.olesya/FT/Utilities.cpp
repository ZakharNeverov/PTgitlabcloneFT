#include "Utilities.hpp"
#include <iostream>
#include <algorithm>
#include <iterator>
#include "Dictionary.hpp"
#include "userCommands.hpp"

namespace shkroba
{
  std::ostream& operator<<(std::ostream& out, std::set< std::string >& set)
  {
    for (auto& item: set)
    {
      out << item << ' ';
    }
    out << '\n';
    return out;
  }

  Dictionary createCommonDictionary(std::vector< Dictionary >& common)
  {
    Dictionary result("result");
    bool isCommon;

    std::map< std::string, size_t > translates;

    Dictionary dictionary = *common.begin();
    for (const auto& pair: dictionary)
    {
      isCommon = true;
      translates.clear();
      for (const auto& dictionarySecond: common)
      {
        if (dictionarySecond.search(pair.first) == dictionarySecond.end())
        {
          isCommon = false;
          break;
        }
        else
        {
          pairER item = *dictionarySecond.search(
            pair.first);
          for (const auto& word: *item.second)
          {
            translates[word]++;
          }
        }
      }
      if (isCommon)
      {
        std::set< std::string > set;
        for (const auto& translate: translates)
        {
          if (translate.second == common.size())
          {
            set.insert(translate.first);
          }
        }
        if (set.empty())
        {
          std::cout << "Write translate for " + pair.first << '\n';
          std::cout << "Write \"Exit\" to escape\n";
          std::string in;
          while (true)
          {
            std::cin >> in;
            if (in == "Exit" && set.empty())
            {
              continue;
            }
            if (in == "Exit")
            {
              break;
            }
            set.insert(in);
          }
        }
        std::shared_ptr< std::set< std::string > > sharedPtr = std::make_shared< std::set< std::string > >(set);
        result.insert({pair.first, sharedPtr});
      }
    }
    return result;
  }

  Dictionary createFromUniqueWords(const Dictionary& d1, const Dictionary& d2)
  {
    Dictionary common("common");
    std::set_intersection(
      d1.getDictionary().begin(),
      d1.getDictionary().end(),
      d2.getDictionary().begin(),
      d2.getDictionary().end(),
      std::inserter(common.getDictionary(), common.getDictionary().begin())
   );
    return common;
  }

  Dictionary createFromOneTranslate(const Dictionary& dictionary)
  {
    std::cout << "Input name of new dictionary" << '\n';
    std::string name;
    std::cin >> name;
    Dictionary newDictionary(name);
    std::copy_if(
      dictionary.begin(),
      dictionary.end(),
      std::inserter(newDictionary.getDictionary(), newDictionary.begin()),
      [](const std::pair< std::string, std::shared_ptr< std::set< std::string > > >& pair)
      { return pair.second->size() == 1; }
    );
    return newDictionary;
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

  void doCommonForTwo(const Dictionary& source, const Dictionary& extra, std::ostream& out)
  {
    Dictionary result;
    std::merge(
      source.getDictionary().begin(),
      source.getDictionary().end(),
      extra.getDictionary().begin(),
      extra.getDictionary().end(),
      std::inserter(result.getDictionary(), result.begin())
    );
    result.addWords(source);
    result.addWords(extra);
    result.printDictionary(out);
  }

  void doOneTranslate(const Dictionary& dictionary, std::ostream& out)
  {
    createFromOneTranslate(dictionary).printDictionary(out);
  }

  void doCreateFromUniqueWords(const Dictionary& first, const Dictionary& second, std::ostream& out)
  {
    createFromUniqueWords(first, second).printDictionary(out);
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
}
