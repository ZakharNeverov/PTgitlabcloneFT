#include <limits>

#include "commands.hpp"

int main()
{
  shulgin::printMenu(std::cout);
  using CrossRefDicts = std::vector< shulgin::CrossReferences >;
  CrossRefDicts lib;
  std::map< std::string, std::function< void() > > commandsList =
  {
    { "read", std::bind(shulgin::doReadCmd, std::ref(lib), std::ref(std::cin)) },
    { "print", std::bind(shulgin::doPrintCmd, std::ref(lib), std::ref(std::cin), std::ref(std::cout)) },
    { "find", std::bind(shulgin::doFindCmd, std::ref(lib), std::ref(std::cin), std::ref(std::cout)) },
    { "remove", std::bind(shulgin::doRemoveCmd, std::ref(lib), std::ref(std::cin), std::ref(std::cout)) },
    { "unite", std::bind(shulgin::doUniteCmd, std::ref(lib), std::ref(std::cin), std::ref(std::cout)) },
    { "cross", std::bind(shulgin::doCrossCmd, std::ref(lib), std::ref(std::cin), std::ref(std::cout)) },
    { "printtext", std::bind(shulgin::doPrintTextCmd, std::ref(std::cin), std::ref(std::cout)) },
    { "removetext", std::bind(shulgin::doRemoveTextCmd, std::ref(lib), std::ref(std::cin), std::ref(std::cout)) }
  };
  while (!std::cin.eof())
  {
    try
    {
      std::string cmd;
      std::cin >> cmd;
      if (std::cin.eof())
      {
        break;
      }
      auto it = commandsList.find(cmd);
      if (it != commandsList.end())
      {
        it->second();
      }
      else
      {
        shulgin::checkStreamOn(std::cin, '\n');
        throw std::logic_error("Error, unknown command!");
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << "\n";
      if (std::cin.fail())
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }
  return 0;
}
