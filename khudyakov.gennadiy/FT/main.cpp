#include <iostream>
#include <fstream>
#include <functional>
#include "MessagesIO.h"
#include "commands.h"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    khudyakov::invalidArgumentMessage(std::cerr);
    return 1;
  }

  using cmd_t = std::function< void(std::string, khudyakov::Dicts_t&) >;
  using constCmd_t = std::function< void(const std::string&, const khudyakov::Dicts_t&) >;
  using cmdDict_t = typename std::map< std::string, cmd_t >;
  using constCmdDict_t = typename std::map< std::string, constCmd_t >;

  cmdDict_t commands
  {
    {"deleteOften", khudyakov::deleteOften},
    {"deleteRare", khudyakov::deleteRare},
    {"deleteWord", khudyakov::deleteWord},
    {"deleteWordsInString", khudyakov::deleteWordsInString},
    {"rename", khudyakov::rename},
    {"complement", khudyakov::complement},
    {"intersect", khudyakov::intersect},
    {"union", khudyakov::unit}
  };

  constCmdDict_t constCommands
  {
    {"print", khudyakov::print},
    {"printWordsInString", khudyakov::printWordsInString},
    {"similarity", khudyakov::similarity}
  };

  try
  {
    khudyakov::Dicts_t dicts;
    for (int i = 1; i < argc; i++)
    {
      std::ifstream fileStream(argv[i]);
      if (!fileStream)
      {
        khudyakov::errorOpenFileMessage(std::cerr);
        return 1;
      }
      khudyakov::Dict_t dict;
      std::string nameOfDict = argv[i];
      size_t numberOfString = 1;
      std::string string = "";
      while (!fileStream.eof())
      {
        std::getline(fileStream, string);
        if (string.empty())
        {
          continue;
        }
        khudyakov::parseStringToDict(string, numberOfString++, dict);
      }
      dicts.emplace(nameOfDict, dict);
    }

    while (!std::cin.eof())
    {
      std::string str = "";
      std::getline(std::cin, str);
      if (str.empty())
      {
        continue;
      }
      try
      {
        auto cmd = khudyakov::getWord(str);
        if (constCommands.find(cmd) != constCommands.cend())
        {
          (*constCommands.find(cmd)).second(str, dicts);
          std::cout << '\n';
        }
        else if (commands.find(cmd) != commands.cend())
        {
          (*commands.find(cmd)).second(str, dicts);
        }
        else
        {
          throw std::logic_error{"<INVALID COMMAND>"};
        }
      }
      catch (const std::logic_error&)
      {
        khudyakov::invalidCommandMessage(std::cout) << '\n';
      }
    }
  }
  catch (const std::exception& er)
  {
    std::cerr << er.what();
    return 1;
  }
}
