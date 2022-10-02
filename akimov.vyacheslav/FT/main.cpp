#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include "Dictionary.hpp"
#include "Dictionary-interaction.hpp"
int main()
{
  try
  {
    std::vector< akimov::Dictionary > dicts;
    std::map< std::string, std::function< void() > > commandsMap =
    {
      {"create", std::bind(akimov::create, std::ref(std::cin), std::ref(dicts))},
      {"print", std::bind(akimov::print, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))},
      {"getMostPopular", std::bind(akimov::getMostPopular, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))},
      {"find", std::bind(akimov::find, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))},
      {"drop", std::bind(akimov::drop, std::ref(std::cin), std::ref(dicts))},
      {"add", std::bind(akimov::add, std::ref(std::cin), std::ref(dicts))},
      {"remove", std::bind(akimov::remove, std::ref(std::cin), std::ref(dicts))},
      {"intersection", std::bind(akimov::intersection, std::ref(std::cin), std::ref(dicts))},
      {"intersectionFromLetter", std::bind(akimov::intersectionFromLetter, std::ref(std::cin), std::ref(dicts))}
    };
    std::string command{};
    while (!std::cin.eof())
    {
      std::cin >> command;
      commandsMap.at(command)();
    }
  }
  catch (const std::invalid_argument& e)
  {
    std::cerr << e.what();
  }
}
