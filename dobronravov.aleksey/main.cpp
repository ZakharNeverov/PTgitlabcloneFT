#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <map>

#include "globalLocale.hpp"
#include "commands.hpp"
#include "stringMethods.hpp"

int main(int argc, char**)
{
  auto local = kurzov::globalLocale(std::locale("ru_RU.utf8"));

  if (argc != 1)
  {
    std::cerr << "Bad args!\n";
    return 1;
  }

  dobronravov::dicts_t dicts;

  using func_t = std::function< void(std::string) >;
  std::map< std::string, func_t > commandsDict
   {
     {"union", std::bind(dobronravov::doUnion, std::placeholders::_1, std::ref(dicts))},
     {"intersect", std::bind(dobronravov::doIntersect, std::placeholders::_1, std::ref(dicts))},
     {"complement", std::bind(dobronravov::doComplement, std::placeholders::_1, std::ref(dicts))},
     {"load", std::bind(dobronravov::doLoad, std::placeholders::_1, std::ref(dicts))},
     {"translate", std::bind(dobronravov::doTranslate, std::placeholders::_1, std::ref(dicts), std::ref(std::cout))},
     {"remove", std::bind(dobronravov::doRemove, std::placeholders::_1, std::ref(dicts))},
     {"createDict", std::bind(dobronravov::doCreateDict, std::placeholders::_1, std::ref(dicts))},
     {"print", std::bind(dobronravov::doPrint, std::placeholders::_1, std::ref(dicts), std::ref(std::cout))}
   };

  while (std::cin)
  {
    std::string command{""};
    std::getline(std::cin, command);
    if (command.empty())
    {
      continue;
    }
    std::string commandName = dobronravov::getWord(command);
    auto commandsDictIter = commandsDict.find(commandName);
    try
    {
      if (commandsDictIter == commandsDict.end())
      {
        throw std::logic_error("Bad command!");
      }
      commandsDictIter->second(command);
    }
    catch (const std::exception& e)
    {
      std::cerr << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
