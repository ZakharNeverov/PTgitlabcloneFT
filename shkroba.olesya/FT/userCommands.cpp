#include "userCommands.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
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

  using mapHelpCommands =  std::map< std::string, std::string >;
  mapHelpCommands defineHelpCommands()
  {
    mapHelpCommands mapHelp =
      {
        {"PRINT", "Print words from the selected dictionary."},
        {"SIZE", "Print size of selected dictionary."},
        {"START", "Print words beginning with input letter from the selected dictionary."},
        {"ONETRANSLATE", "Create the dictionary and print words from it with one translation."},
        {"FROMTWO", "Make dictionary from the current dictionary and test dictionary from program."},
        {"UNIQUE", "Create common dictionary from nonrepeating words from current and test dictionaries from program."},
        {"COMMON", "Create common dictionary with input number of dictionaries and print it."},
      };
    return mapHelp;
  }
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
      throw std::logic_error("<INCORRECT NAME>");
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
      throw std::logic_error("<INCORRECT NAME>");
    }
  }

  void makeFindWord(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    std::string string;
    std::getline(in >> std::ws, string);
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
      throw std::logic_error("<INCORRECT NAME>");
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
      Dictionary::createFromOneTranslate(*iter).printDictionary(out);
      out << '\n';
    }
    else
    {
      throw std::logic_error("<INCORRECT NAME>");
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
      Dictionary::createFromUniqueWords(*iter, test).printDictionary(out);
      out << '\n';
    }
    else
    {
      throw std::logic_error("<INCORRECT NAME>");
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
      Dictionary::commonForTwo(*iter, test, std::cout);
      out << '\n';
    }
    else
    {
      throw std::logic_error("<INCORRECT NAME>");
    }
  }

  void makeCommonDictionary(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    std::string string;
    std::getline(in >> std::ws , string);
    std::vector< Dictionary > dictionaries;
    while (string != "")
    {
      std::string current;
      getNextWord(string, current);
      auto iter = std::find_if(base.begin(), base.end(), std::bind(isName(), _1, current));
      if (iter != base.end())
      {
        dictionaries.push_back(*iter);
      }
      else
      {
        throw std::logic_error("<INCORRECT NAME>");
      }
    }
    shkroba::iofmtguard iofmtguard(out);
    if (dictionaries.size() == 1)
    {
      throw std::logic_error("<INCORRECT NUMBER OF ARGUMENTS>");
    }
    createCommonDictionary(dictionaries).printDictionary(std::cout);
    out << '\n';
  }

  void makePrintFile(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
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
      std::cout << "Write the name of the output file" << '\n';
      std::string fileName;
      std::cin >> fileName;
      shkroba::iofmtguard iofmtguard(out);
      shkroba::doPrintInFile(*iter, fileName);
      out << '\n';
    }
    else
    {
      throw std::logic_error("<INCORRECT NAME>");
    }
  }

  void makeHelp(std::istream& in, std::ostream& out)
  {
    std::string command;
    in >> command;
    iofmtguard guard(out);
    if (!isTwoWordsInCommand(in))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    auto commandsHelp = defineHelpCommands();
    auto iter = commandsHelp.find(command);
    if (iter == commandsHelp.end())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    std::cout << iter->second;
  }
}
