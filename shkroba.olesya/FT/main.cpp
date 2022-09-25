#include <limits>
#include <string>
#include <functional>
#include <iostream>
#include <fstream>
#include "Utilities.hpp"
#include "userCommands.hpp"

namespace
{
  std::istream& makeIstreamClean(std::istream& in)
  {
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return in;
  }
  using mapCommands = std::map< std::string, std::function< void() > >;
  mapCommands allCommands(std::istream& in, std::vector< shkroba::Dictionary >& base, std::ostream& out)
  {
    mapCommands mapOfCommands =
      {
        {"PRINT", std::bind(shkroba::makePrint, std::ref(in), std::ref(base), std::ref(out))},
        {"SIZE", std::bind(shkroba::makeSize, std::ref(in), std::ref(base), std::ref(out))},
        {"START", std::bind(shkroba::makeFindWord, std::ref(in), std::ref(base), std::ref(out))},
        {"ONETRANSLATE", std::bind(shkroba::makeOneTranslate, std::ref(in), std::ref(base), std::ref(out))},
        {"FROMTWO", std::bind(shkroba::makeCommonForTwo, std::ref(in), std::ref(base), std::ref(out))},
        {"UNIQUE", std::bind(shkroba::makeUnique, std::ref(in), std::ref(base), std::ref(out))},
        {"COMMON", std::bind(shkroba::makeCommonDictionary, std::ref(in), std::ref(base), std::ref(out))},
        {"WRITE", std::bind(shkroba::makePrintFile, std::ref(in), std::ref(base), std::ref(out))},
        {"HELP", std::bind(shkroba::makeHelp, std::ref(in), std::ref(out))}
      };
    return mapOfCommands;
  }
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Bad args\n";
    return 1;
  }
  std::ifstream fin(argv[1]);
  if (!fin.is_open())
  {
    throw std::invalid_argument("File is not open\n");
  }
  std::vector< shkroba::Dictionary > dictionaries = shkroba::createDictionariesFromFile(fin);
  auto myCommands = allCommands(std::cin, dictionaries, std::cout);
  while (!std::cin.eof())
  {
    std::string command;
    std::cin >> command;
    if (!command.empty())
    {
      try
      {
        auto iter = myCommands.find(command);
        if (iter != myCommands.end())
        {
          iter->second();
        }
        else
        {
          throw std::invalid_argument("<INVALID COMMAND>");
        }
        std::cout << '\n';
      }
      catch (const std::exception& e)
      {
        std::cout << e.what();
        makeIstreamClean(std::cin);
      }
    }
  }
  return 0;
}
