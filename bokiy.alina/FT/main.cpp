#include "clearStream.hpp"
#include "commands.hpp"

int main()
{
  bokiy::maps_t maps;
  std::map< std::string, std::function< void() > > commandsMap =
  {
    {"make", std::bind(bokiy::make, std::ref(std::cin), std::ref(maps))},
    {"write", std::bind(bokiy::write, std::ref(std::cin), std::ref(maps))},
    {"read", std::bind(bokiy::read, std::ref(std::cin), std::ref(maps))},
    {"makeFrequent", std::bind(bokiy::makeFrequentOrRarest, std::ref(std::cin), std::ref(maps), bokiy::getFrequentElement)},
    {"makeRarest", std::bind(bokiy::makeFrequentOrRarest, std::ref(std::cin), std::ref(maps), bokiy::getRarestElement)},
    {"complement", std::bind(bokiy::complementOrIntersect , std::ref(std::cin), std::ref(maps), bokiy::isNotSame)},
    {"intersect", std::bind(bokiy::complementOrIntersect , std::ref(std::cin), std::ref(maps), bokiy::isSame)},
    {"print", std::bind(bokiy::print , std::ref(std::cin), std::ref(maps))},
    {"printFrequentWord", std::bind(bokiy::printFrequentOrRarestWord , std::ref(std::cin), std::ref(maps), bokiy::getFrequentElement)},
    {"printRarestWord", std::bind(bokiy::printFrequentOrRarestWord , std::ref(std::cin), std::ref(maps), bokiy::getRarestElement)}
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
        bokiy::clearIStream(std::cin);
      }
    }
  }
  return 0;
}
