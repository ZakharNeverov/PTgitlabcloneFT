#include <iostream>
#include <functional>

#include "Commands.hpp"
#include "Messages.hpp"

int main()
{
  std::locale::global(std::locale(".utf8"));
  using funcObj = std::function< void(std::ostream&, yermakov::TextDict&, std::string&) >;

  std::map< std::string, funcObj > commandsArray
  {
    {"GET", yermakov::doGet},
    {"WRITE", yermakov::doWrite},
    {"CALCULATE", yermakov::doCalculateSize},
    {"PRINT", yermakov::doPrint},
    {"COMPRESS", yermakov::doCompress},
    {"DECOMPRESS", yermakov::doDecompress},
    {"EFFICIENCY", yermakov::doEfficiency},
    {"CONCAT", yermakov::doConcat},
    {"CUTOFF", yermakov::doCut}
  };

  yermakov::TextDict texts;

  while (!std::cin.eof())
  {
    try
    {
      std::string description;
      std::getline(std::cin, description);
      if (description.empty())
      {
        break;
      }
      std::string command = description.substr(0, description.find_first_of(" "));
      std::string argument = description.substr(description.find_first_of(" ") + 1);
      commandsArray.at(command)(std::cout, texts, argument);
    }
    catch (...)
    {
      yermakov::printInvalid(std::cout) << "\n";
    }
  }
  return 0;
}
