#include "userCommands.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include "Utilities.hpp"
#include "iofmtguard.hpp"
#include "Dictionary.hpp"
#include "forRead.hpp"

namespace
{
  bool isTwoWordsInCommand(std::istream& in)
  {
    shkroba::iofmtguard iofmtguard(in);
    in >> std::noskipws >> shkroba::DelimiterIO { '\n' };
    return static_cast< bool >(in);
  }

  void getNextWord(std::string& fromStr, std::string& toStr)
  {
    size_t closestSpace = fromStr.find_first_of(' ');
    toStr = fromStr.substr(0, closestSpace);
    size_t countForDelete = (closestSpace == std::string::npos) ? closestSpace : closestSpace + 1;
    fromStr.erase(0, countForDelete);
  }

  struct isName
  {
    bool operator()(shkroba::Dictionary& dictionary, std::string name)
    {
      return dictionary.getName() == name;
    }
  };
}

namespace shkroba
{
  using namespace std::placeholders;

  void makePrint(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    std::string word;
    in  >> word;
    iofmtguard guard(out);
    if (!isTwoWordsInCommand(in))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    auto iter = std::find_if(base.begin(), base.end(), std::bind(isName(), _1, word));
    if (iter != base.end())
    {
      shkroba::iofmtguard iofmtguard(out);
      shkroba::doPrintDictionary(*iter, out);
      out << '\n';
    }
    else
    {
      shkroba::iofmtguard iofmtguard(out);
      out << "No dictionaries with such name" << '\n';
    }
  }

  void makeSize(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    std::string word;
    in >> word;
    iofmtguard guard(out);
    if (!isTwoWordsInCommand(in))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    auto iter = std::find_if(base.begin(), base.end(), std::bind(isName(), _1, word));
    if (iter != base.end())
    {
      shkroba::iofmtguard iofmtguard(out);
      shkroba::doSize(*iter, out);
      out << '\n';
    }
    else
    {
      shkroba::iofmtguard iofmtguard(out);
      out << "No dictionaries with such name" << '\n';
    }
  }

  void makeFindWord(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    std::string string;
    std::getline(in >> std::ws , string);
    std::string letter;
    getNextWord(string, letter);
    if (letter.size() != 1)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    std::string name;
    getNextWord(string, name);
    auto iter = std::find_if(base.begin(), base.end(), std::bind(isName(), _1, name));
    if (iter != base.end())
    {
      shkroba::iofmtguard iofmtguard(out);
      shkroba::doFindWord(*iter, letter, out);
      out << '\n';
    }
    else
    {
      shkroba::iofmtguard iofmtguard(out);
      out << "No dictionaries with such name" << '\n';
    }
  }

  void makeOneTranslate(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    std::string word;
    in >> word;
    iofmtguard guard(out);
    if (!isTwoWordsInCommand(in))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    auto iter = std::find_if(base.begin(), base.end(), std::bind(isName(), _1, word));
    if (iter != base.end())
    {
      shkroba::iofmtguard iofmtguard(out);
      shkroba::doOneTranslate(*iter, out);
      out << '\n';
    }
    else
    {
      shkroba::iofmtguard iofmtguard(out);
      out << "No dictionaries with such name" << '\n';
    }
  }

  void makeUnique(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    std::string word;
    in >> word;
    iofmtguard guard(out);
    if (!isTwoWordsInCommand(in))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    shkroba::Dictionary test;
    in >> test;
    auto iter = std::find_if(base.begin(), base.end(), std::bind(isName(), _1, word));
    if (iter != base.end())
    {
      shkroba::iofmtguard iofmtguard(out);
      shkroba::doCreateFromUniqueWords(*iter, test, std::cout);
      out << '\n';
    }
    else
    {
      shkroba::iofmtguard iofmtguard(out);
      out << "No dictionaries with such name" << '\n';
    }
  }

  void makeCommonForTwo(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    std::string word;
    in >> word;
    iofmtguard guard(out);
    if (!isTwoWordsInCommand(in))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    shkroba::Dictionary test;
    in >> test;
    auto iter = std::find_if(base.begin(), base.end(), std::bind(isName(), _1, word));
    if (iter != base.end())
    {
      shkroba::iofmtguard iofmtguard(out);
      shkroba::doCommonForTwo(*iter, test, std::cout);
      out << '\n';
    }
    else
    {
      shkroba::iofmtguard iofmtguard(out);
      out << "No dictionaries with such name" << '\n';
    }
  }

  void makeCommonDictionary(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    std::string string;
    std::getline(in >> std::ws , string);
    std::vector< Dictionary > dictionaries;
    while(string != "")
    {
      std::string current;
      getNextWord(string, current);
      auto iter = std::find_if(base.begin(), base.end(), std::bind(isName(), _1, current));
      if (iter != base.end())
      {
        dictionaries.push_back(*iter);
      }
    }
    shkroba::iofmtguard iofmtguard(out);
    createCommonDictionary(dictionaries).printDictionary(std::cout);
    out << '\n';
  }

  void makeHelp(std::istream& in, std::ostream& out)
  {
    std::string word;
    in >> word;
    iofmtguard guard(out);
    if (!isTwoWordsInCommand(in))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    if (word == "PRINT")
    {
      out << "Print words from the selected dictionary." << '\n';
    }
    else if (word == "SIZE")
    {
      out << "Print size of selected dictionary." << '\n';
    }
    else if (word == "ONETRANSLATE")
    {
      out << "Create the dictionary and print words from the selected dictionary that have one translation.";
      out << '\n';
    }
    else if (word == "START")
    {
      out << "Print words beginning with input letter from the selected dictionary."<< '\n';
    }
    else if (word == "ADDANOTHER")
    {
      out << "Make dictionary from the current dictionary and test dictionary from program." << '\n';
      out << "Print it." << '\n';
    }
    else if (word == "UNIQUE")
    {
      out << "Create common dictionary from words that do not repeat in current dictionary and in test dictionary from program.";
      out << "Print it." << '\n';
    }
    else if (word == "COMMON")
    {
      out << "Create common dictionary with input number of dictionaries and print it." << '\n';
    }
    else
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
}
