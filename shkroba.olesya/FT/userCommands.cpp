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
}

namespace shkroba
{
  using namespace std::placeholders;

  void makePrint(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    for (const auto& current: base)
    {
      shkroba::iofmtguard iofmtguard(out);
      shkroba::doPrintDictionary(current, std::cout);
      out << '\n';
    }
  }

  void makeSize(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    for (const auto& current: base)
    {
      shkroba::iofmtguard iofmtguard(out);
      out << current.getName() <<'\n';
      shkroba::doSize(current, std::cout);
      out << '\n';
    }
  }

  void makeFindWord(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    char letter;
    in >> letter;
    if (!isTwoWordsInCommand(in))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    for (const auto& current: base)
    {
      out << current.getName()<<'\n';
      shkroba::iofmtguard iofmtguard(out);
      shkroba::doFindWord(current, letter,out);
      out << '\n';
    }
  }

  void makeOneTranslate(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    for (const auto& current: base)
    {
      shkroba::iofmtguard iofmtguard(out);
      shkroba::doOneTranslate(current, out);
      out << '\n';
    }
  }

  void makeUnique(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    shkroba::Dictionary test;
    in >> test;
    for (const auto& current: base)
    {
      shkroba::iofmtguard iofmtguard(out);
      shkroba::doCreateFromUniqueWords(current, test, std::cout);
      out << '\n';
    }
  }

  void makeCommonForTwo(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    shkroba::Dictionary test;
    in >> test;
    for (const auto &current: base)
    {
      out << current.getName();
      shkroba::iofmtguard iofmtguard(out);
      shkroba::doCommonForTwo(current, test, std::cout);
      out << '\n';
    }
  }

  void makeCommonDictionary(std::istream& in, std::vector< Dictionary >& base, std::ostream& out)
  {
    std::string number;
    in >> number;
    if (!isTwoWordsInCommand(in) || !std::stoul(number))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    size_t count = std::stoul(number);
    if (count > base.size())
    {
      throw std::invalid_argument("Error! There are only 5 dictionaries.");
    }
    std::vector< Dictionary > dictionaries;
    for (size_t i = 0; i < count; i++)
    {
      dictionaries.push_back(base[i]);
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
