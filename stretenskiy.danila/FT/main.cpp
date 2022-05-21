#include <iostream>
#include <map>
#include <functional>
#include "command-system.hpp"

int main()
{
  using namespace stretenskiy::function;

  std::vector< std::string > name;
  std::vector< myDict > hashT;

  using namespace std::placeholders;
  std::map< std::string, std::function< void(std::ostream &) > > commands(
  {
    {"CREATION", std::bind(creationDict, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"ADD", std::bind(add, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"REMOVE", std::bind(removeWord, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"PRINT", std::bind(print, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"SEARCH", std::bind(search, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"CLEAR", std::bind(clearDict, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"UNION", std::bind(unionDict, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))},
    {"INTERSECTION", std::bind(intersectDict, _1, std::ref(hashT), std::ref(name), std::ref(std::cin))}
  });

  while (!std::cin.eof())
  {
    std::cin.clear();
    std::string command;
    std::cin >> command;
    auto iter = commands.find(command);
    if (iter != commands.end())
    {
      try
      {
        iter->second(std::cout);
      }
      catch (const std::exception &e)
      {
        std::cout << "Not find element\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }
  return 0;
}
