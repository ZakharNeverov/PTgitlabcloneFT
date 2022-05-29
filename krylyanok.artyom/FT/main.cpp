#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <algorithm>
#include <iterator>
#include <functional>
#include <map>
#include <list>
#include "operations.hpp"
#include "hash-dictionary.hpp"
#include "commands.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Bad args\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "File is not open\n";
    return 1;
  }
  using istreamIter_t = std::istream_iterator< krylyanok::Dictionary >;
  std::vector< krylyanok::Dictionary > dictionaries;
  while (!file.eof())
  {
    file.clear();
    std::string nameDict = "";
    std::string nameFile = "";
    file >> nameDict >> nameFile;
    std::vector< std::string > words;
    try
    {
      krylyanok::makeDict(nameFile, words);
    }
    catch (const std::exception&)
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      continue;
    }
    dictionaries.push_back({ words.size(), nameDict });
    for (std::string el : words)
    {
      dictionaries[dictionaries.size() - 1].add(el);
    }
  }
  file.close();
  krylyanok::Commands commStruct(dictionaries, std::cin, std::cout);
  std::map< std::string, std::function< void(void) > > commands(
    {
      {"LOAD", std::bind(&krylyanok::Commands::getLoad, std::ref(commStruct))},
      {"PRINT", std::bind(&krylyanok::Commands::getPrint, std::ref(commStruct))},
      {"TOP", std::bind(&krylyanok::Commands::getTop, std::ref(commStruct))},
      {"SEARCH", std::bind(&krylyanok::Commands::getSearch, std::ref(commStruct))},
      {"MERGE", std::bind(&krylyanok::Commands::getMerge, std::ref(commStruct))},
      {"DELETE", std::bind(&krylyanok::Commands::getDelete, std::ref(commStruct))}
    });
  while (!std::cin.eof())
  {
    std::string nameOfCommand = "";
    std::cin >> nameOfCommand;
    if (nameOfCommand.empty())
    {
      continue;
    }
    try
    {
      commands.at(nameOfCommand)();
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
