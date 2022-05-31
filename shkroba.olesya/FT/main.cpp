#include <limits>
#include <string>
#include <functional>
#include <iostream>
#include <fstream>
#include "Utilities.hpp"
#include "userCommands.hpp"

namespace
{
  void makeIstreamClean(std::istream& in)
  {
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  using mapCommandsType =  std::map< std::string, std::function< void() > >;
  mapCommandsType defineAllCommands(std::istream& in, std::vector<shkroba::Dictionary>& base,std::ostream& out)
  {
    mapCommandsType mapOfCommands =
      {
        { "PRINT",std::bind(shkroba::makePrint, std::ref(in), std::ref(base), std::ref(out))},
        { "SIZE",std::bind(shkroba::makeSize, std::ref(in), std::ref(base), std::ref(out)) },
        { "START", std::bind(shkroba::makeFindWord, std::ref(in), std::ref(base), std::ref(out)) },
        { "ONETRANSLATE", std::bind(shkroba::makeOneTranslate, std::ref(in), std::ref(base), std::ref(out)) },
        { "FROMTWO", std::bind(shkroba::makeCommonForTwo, std::ref(in), std::ref(base), std::ref(out)) },
        { "UNIQUE", std::bind(shkroba::makeUnique, std::ref(in), std::ref(base), std::ref(out)) },
        { "COMMON", std::bind(shkroba::makeCommonDictionary, std::ref(in), std::ref(base), std::ref(out)) },
        { "HELP", std::bind(shkroba::makeHelp, std::ref(in), std::ref(out)) }
      };
    return mapOfCommands;
  }
}

int main(int argc, char**argv)
{
// int main(int argc, char** argv)
//{
//  if (argc != 2)
//  {
//    std::cerr << "Bad args\n";
//    return 1;
//  }

  std::ifstream fin("in.txt");
//  std::ifstream fin(argv[1]);
  if (!fin.is_open())
  {
    std::cerr << "File is not open\n";
    return 1;
  }
  std::vector< shkroba::Dictionary > dictionaries = shkroba::createDictionariesFromFile(fin);
  auto myCommands = defineAllCommands(std::cin, dictionaries, std::cout);
  while (!std::cin.eof())
  {
    std::string command;
    std::cin >> command;
    if(!command.empty())
    {
      try
      {
        auto iter = myCommands.find(command);
        iter->second();
        std::cout << '\n';
      }
      catch (const std::exception &e)
      {
        std::cout << "<INVALID COMMAND>" << '\n';
      }
      if ((std::cin.fail() && !std::cin.eof()) || myCommands.find(command) == myCommands.end())
      {
        makeIstreamClean(std::cin);
      }
    }
  }
  return 0;
}
