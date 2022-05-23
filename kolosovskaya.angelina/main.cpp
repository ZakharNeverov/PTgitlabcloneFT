#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <locale>
#include <exception>
#include <functional>
#include "Commands.h"
#include "IoOperations.h"
#include "AdditionalCommands.h"

int main()
{
  std::locale local = std::locale::global(std::locale("ru_RU.utf8"));
  std::string str = "";
  std::getline(std::cin, str);
  kolosovskaya::Dicts allDicts;
  try
  {
    allDicts = kolosovskaya::getDicts(str, std::cout);
  }
  catch (const std::exception& e)
  {
    std::cout << e.what();
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
