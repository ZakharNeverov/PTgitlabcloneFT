#include <iostream>
#include <string>
#include <map>
#include <functional>
#include "CommandsWithDict.hpp"

namespace gavr = gavrikov;

int main()
{
  gavr::dict_t dicts;
  std::map< std::string, std::function< void(void) > > commands
  {
    {"createDict", std::bind(gavr::createDict, std::ref(dicts), std::ref(std::cin))},
    {"printDict", std::bind(gavr::printDict, std::ref(dicts), std::ref(std::cin))},
    {"insertWord", std::bind(gavr::insertWord, std::ref(dicts), std::ref(std::cin))},
    {"intersection", std::bind(gavr::intersection, std::ref(dicts), std::ref(std::cin))},
    {"complement", std::bind(gavr::complement, std::ref(dicts), std::ref(std::cin))},
    {"countPref", std::bind(gavr::countPref, std::ref(dicts), std::ref(std::cin))},
    {"dropDict", std::bind(gavr::dropDict, std::ref(dicts), std::ref(std::cin))},
    {"unload", std::bind(gavr::unload, std::ref(dicts), std::ref(std::cin))},
    {"load", std::bind(gavr::load, std::ref(dicts), std::ref(std::cin))}
  };
  std::string commandNow{};
  while (!std::cin.eof())
  {
    std::cin >> commandNow;
    try
    {
      commands.at(commandNow)();
    }
    catch (const std::logic_error& e)
    {
      std::cerr << e.what() << "\n";
    }
  }
}
