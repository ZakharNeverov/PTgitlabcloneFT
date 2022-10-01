#include <iostream>
#include <limits>
#include <functional>
#include <memory>
#include "ioctl.hpp"
#include "commands.hpp"

int main()
{
  std::vector< abraamyan::Table > tableArray;
  std::map< std::string, std::function< void() > > commands(
  {
    { "create", std::bind(abraamyan::createCommand, std::ref(tableArray)) },
    { "delete", std::bind(abraamyan::deleteCommand, std::ref(tableArray)) },
    { "print", std::bind(abraamyan::printCommand, std::ref(tableArray)) },
    { "printToFile", std::bind(abraamyan::printToFileCommand, std::ref(tableArray)) },
    { "read", std::bind(abraamyan::readFileCommand, std::ref(tableArray)) },
    { "union", std::bind(abraamyan::unionCommand, std::ref(tableArray)) },
    { "intersection", std::bind(abraamyan::intersectionCommand, std::ref(tableArray)) },
    { "search", std::bind(abraamyan::searchCommand, std::ref(tableArray)) },
    { "searchEverywhere", std::bind(abraamyan::searchEverywhereCommand, std::ref(tableArray)) },
    { "count", std::bind(abraamyan::countCommand, std::ref(tableArray)) },
    { "countEverywhere", std::bind(abraamyan::countEverywhereCommand, std::ref(tableArray)) },
    { "copy", std::bind(abraamyan::copyCommand, std::ref(tableArray)) },
    { "list", std::bind(abraamyan::listCommand, std::ref(tableArray)) }
  });

  while (!std::cin.eof())
  {
    try
    {
      std::string command;
      std::cin >> abraamyan::CommandIO{ command };
      if (!std::cin)
      {
        continue;
      }
      commands.at(command)();
    }
    catch (const std::logic_error &e)
    {
      std::cout << "<INVALID COMMAND>\n";
      if (!std::cin.eof())
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }
  return 0;
}
