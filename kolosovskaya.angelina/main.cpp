#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <locale>
#include <fstream>
#include <exception>
#include <functional>
#include "Commands.h"
#include "IoOperations.h"
#include "AdditionalCommands.h"

int main(int argc, char** argv)
{
  std::locale local = std::locale::global(std::locale("ru_RU.utf8"));
  kolosovskaya::Dicts allDicts;
  for (int i = 1; i < argc; ++i)
  {
    std::string fileName = argv[i];
    std::ifstream fin(fileName);
    if (!fin.is_open())
    {
      std::cerr << "File isn't open!\n";
      return 1;
    }
    std::string dictName = fileName.substr(0, fileName.find_first_of('.'));
    kolosovskaya::Dict newDict = kolosovskaya::makeDict(fin, std::cout);
    if (kolosovskaya::doesExist(dictName, allDicts))
    {
      kolosovskaya::printExistedDictMessage(std::cout);
      return 2;
    }
    allDicts[dictName] = newDict;
  }
  std::map< std::string, std::function< void() > > commandsDict =
  {
    {"print", std::bind(kolosovskaya::doPrint, std::ref(allDicts), std::ref(std::cin), std::ref(std::cout))},
    {"complement", std::bind(kolosovskaya::doComplement, std::ref(allDicts), std::ref(std::cin), std::ref(std::cout))},
    {"makeUnion", std::bind(kolosovskaya::doUnion, std::ref(allDicts), std::ref(std::cin), std::ref(std::cout))},
    {"load", std::bind(kolosovskaya::doLoad, std::ref(allDicts), std::ref(std::cin), std::ref(std::cout))},
    {"viewDicts", std::bind(kolosovskaya::doViewDicts, std::ref(allDicts), std::ref(std::cin), std::ref(std::cout))},
    {"translate", std::bind(kolosovskaya::doTranslate, std::ref(allDicts), std::ref(std::cin), std::ref(std::cout))},
    {"add", std::bind(kolosovskaya::doAdd, std::ref(allDicts), std::ref(std::cin), std::ref(std::cout))},
  };
  while (!std::cin.eof())
  {
    try
    {
      std::cin.clear();
      std::string command = "";
      std::cin >> command;
      if (command.empty())
      {
        continue;
      }
      auto iter = commandsDict.find(command);
      if (iter == commandsDict.end())
      {
        throw std::logic_error("<INVALID COMMAND>");
      }
      iter->second();
    }
    catch (const std::exception&)
    {
      kolosovskaya::printWrongCommandMessage(std::cout);
    }
  }
  return 0;
}
