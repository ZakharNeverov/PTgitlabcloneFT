#include <iostream>
#include <fstream>
#include <functional>
#include "commands.hpp"
#include "globalLocale.hpp"
#include "AdditionalStringMethods.hpp"

int main(int argc, char** argv)
{
  auto local = kurzov::globalLocale(std::locale("ru_RU.utf8"));

  kurzov::dicts_t dicts;

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
  using command_t = std::function< void(const std::string&) >;
  std::map< std::string, command_t > commands(
   {
     {"print", std::bind(kurzov::doPrint, _1, std::ref(dicts), std::ref(std::cout))},
     {"union", std::bind(kurzov::doUnion, _1, std::ref(dicts))},
     {"complement", std::bind(kurzov::doComplement, _1, std::ref(dicts))},
     {"intersect", std::bind(kurzov::doIntersect, _1, std::ref(dicts))},
     {"load", std::bind(kurzov::doLoad, _1, std::ref(dicts))},
     {"save", std::bind(kurzov::doSave, _1, std::ref(dicts))},
     {"translate", std::bind(kurzov::doTranslate, _1, std::ref(dicts), std::ref(std::cout))},
   });

  while (!std::cin.eof())
  {
    std::cin.clear();
    std::string command_name = "";
    std::cin >> command_name;
    if (!command_name.empty())
    {
      auto command_iter = commands.find(command_name);
      std::string command = "";
      std::getline(std::cin, command);
      try
      {
        if (command_iter == commands.end())
        {
          throw std::invalid_argument("Bad command name");
        }
        command_iter->second(command);
      }
      catch (const std::exception&)
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
  }

  return 0;
}
