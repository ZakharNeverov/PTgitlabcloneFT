#include "clearStream.hpp"
#include "Commands.hpp"

int main()
{
  bulgakov::maps_t maps;
  std::map< std::string, std::function< void() > > commandsMap =
  {
    {"analyze", std::bind(bulgakov::analyze, std::ref(std::cin), std::ref(maps))},
    {"remove", std::bind(bulgakov::remove, std::ref(std::cin), std::ref(maps))},
    {"merge", std::bind(bulgakov::mergeDir, std::ref(std::cin), std::ref(maps))},
    {"unique", std::bind(bulgakov::doUniqDict, std::ref(std::cin), std::ref(maps), bulgakov::isNotSame)},
    {"frequent", std::bind(bulgakov::doFreqDictRare, std::ref(std::cin), std::ref(maps), bulgakov::getFreqEl)},
    {"rare", std::bind(bulgakov::doFreqDictRare, std::ref(std::cin), std::ref(maps), bulgakov::getRareEl)},
    {"print", std::bind(bulgakov::print , std::ref(std::cin), std::ref(maps), std::ref(std::cout))},
    {"search", std::bind(bulgakov::searchWord, std::ref(std::cin), std::ref(maps), std::ref(std::cout))},
  };
  while (!std::cin.eof())
  {
    std::string command = "";
    std::cin >> command;
    if (!command.empty())
    {
      try
      {
        commandsMap.at(command)();
      }
      catch (const std::exception& e)
      {
        std::cerr << e.what() << '\n';
        bulgakov::clearIStream(std::cin);
      }
    }
  }
  return 0;
}
