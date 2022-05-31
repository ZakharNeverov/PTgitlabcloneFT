#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <iterator>
#include <map>
#include <functional>
#include "operations.hpp"
#include "commands.hpp"
#include "dictionary.hpp"
#include "errorhandler.hpp"

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
  std::vector< krylyanok::NameMap > dicts;
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
      krylyanok::streamClean(file);
      continue;
    }
    dicts.push_back({nameDict});
    krylyanok::createDict(words, dicts[dicts.size() - 1]);
  }
  file.close();
  krylyanok::Commands commStruct(dicts, std::cin, std::cout);
  std::map< std::string, std::function< void(void) > > commands(
    {
      {"LOAD", std::bind(&krylyanok::Commands::getLoad, std::ref(commStruct))},
      {"PRINT", std::bind(&krylyanok::Commands::getPrint, std::ref(commStruct))},
      {"TOP", std::bind(&krylyanok::Commands::getTop, std::ref(commStruct))},
      {"SEARCH", std::bind(&krylyanok::Commands::getSearch, std::ref(commStruct))},
      {"MERGE", std::bind(&krylyanok::Commands::getMerge, std::ref(commStruct))},
      {"DELETE", std::bind(&krylyanok::Commands::getDelete, std::ref(commStruct))},
      {"SAVE", std::bind(&krylyanok::Commands::getSave, std::ref(commStruct))},
      {"INSERTWORD", std::bind(&krylyanok::Commands::getInsertWord, std::ref(commStruct))},
      {"DELETEWORD", std::bind(&krylyanok::Commands::getDeleteWord, std::ref(commStruct))}
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
      krylyanok::streamClean(std::cin);
    }
  }
  return 0;
}
