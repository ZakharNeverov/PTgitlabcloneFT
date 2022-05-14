#include <iostream>
#include <fstream>
#include <functional>
#include <limits>
#include "io_dicts.hpp"
#include "processing_dicts.hpp"
#include "commands.hpp"
#include "globalLocale.hpp"
#include "CommandsMessages.hpp"
#include "AdditionalStringMethods.hpp"

int main(int argc, char** argv)
{
  auto local = kurzov::globalLocale(std::locale("ru_RU.utf8"));

  kurzov::dicts_t dicts;

  kurzov::loadNewDict("dict_1", "dict_1.txt", dicts);
  kurzov::printDict(dicts.at("dict_1"), std::cout);
  std::cout << "==========\n";
  kurzov::loadNewDict("dict_2", "dict_2.txt", dicts);
  kurzov::printDict(dicts.at("dict_2"), std::cout);
  std::cout << "==========\n";
  kurzov::loadNewDict("dict_3", "dict_3.txt", dicts);
  kurzov::printDict(dicts.at("dict_3"), std::cout);
  std::cout << "==========\n";

  for (int i = 1; i < argc; ++i)
  {
    std::string newdict_filename = argv[i];
    std::string newdict_name = kurzov::getNameOfFile(newdict_filename);
    bool is_loaded = kurzov::loadNewDict(newdict_name, newdict_filename, dicts);
    if (!is_loaded)
    {
      std::cerr << "Bad filename!\n";
      return 1;
    }
  }

  using namespace std::placeholders;
  using command_t = std::function< bool(kurzov::istream_t&) >;
  std::map< std::string, command_t > commands({
      {"print", std::bind(kurzov::doPrint, _1, std::ref(dicts), std::ref(std::cout))},
      //  {"union", std::bind(kurzov::doUnion, _1, std::ref(dicts))},
      //  {"comlement", std::bind(kurzov::doComplement, _1, std::ref(dicts))},
      //  {"intersect", std::bind(kurzov::doIntersect, _1, std::ref(dicts))},
      //  {"load", std::bind(kurzov::doLoad, _1, std::ref(dicts))},
      //  {"translate", std::bind(kurzov::doTranslate, _1, std::ref(dicts), std::ref(std::cout))},
  });

  while (!std::cin.eof())
  {
    std::cin.clear();
    std::string command_name = "";
    std::cin >> command_name;
    if (!command_name.empty())
    {
      auto command_iter = commands.find(command_name);
      if (command_iter != commands.end() && command_iter->second(std::cin))
      {
        std::cout << '\n';
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
        if ((std::cin.fail() && !std::cin.eof()) || command_iter == commands.end())
        {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
      }
    }
  }

  return 0;
}
