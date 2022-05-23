#include <iostream>
#include <functional>
#include "IOActions.hpp"
#include "OneTypedCommand.hpp"
#include "CommandMessages.hpp"

int main()
{
  maistrenko::Dicts dicts = maistrenko::Dicts();
  std::string cmd = "";
  std::map< std::string, std::function< void() > > exec
  {
    {"CREATE", std::bind(maistrenko::createDict, std::ref(cmd), std::ref(std::cout), std::ref(dicts))},
    {"GET", std::bind(maistrenko::getDict, std::ref(cmd), std::ref(std::cin), std::ref(std::cout), std::ref(dicts))},
    {"TRANSLATE", std::bind(maistrenko::getTranslations, std::ref(cmd), std::ref(std::cout), std::ref(dicts))},
    {"PRINT", std::bind(maistrenko::outputExactDict, std::ref(cmd), std::ref(std::cout), std::ref(dicts))},
    {"ADD", std::bind(maistrenko::addNewWord, std::ref(cmd), std::ref(std::cout), std::ref(dicts))},
    {"UNION", std::bind(maistrenko::unionDictionaries, std::ref(cmd), std::ref(std::cout), std::ref(dicts))},
    {"INTERSECT", std::bind(maistrenko::intersectDictionaries, std::ref(cmd), std::ref(std::cout), std::ref(dicts))},
    {"SUBSTRACT", std::bind(maistrenko::substractDictionaries, std::ref(cmd), std::ref(std::cout), std::ref(dicts))},
    {"DELETE_DICT", std::bind(maistrenko::deleteDict, std::ref(cmd), std::ref(std::cout), std::ref(dicts))},
    {"DELETE_WORD", std::bind(maistrenko::deleteWord, std::ref(cmd), std::ref(std::cout), std::ref(dicts))},
    {"DELETE_TRANS", std::bind(maistrenko::deleteTranslation, std::ref(cmd), std::ref(std::cout), std::ref(dicts))}
  };


  while (!std::cin.eof())
  {
    cmd = "";
    std::getline(std::cin, cmd);
    if (!cmd.empty())
    {
      std::string command = "";
      if (maistrenko::getNextWord(cmd, ' ', command))
      {
        try
        {
          exec.at(command)();
        }
        catch (const std::out_of_range&)
        {
          maistrenko::notifyUnexistingCommand(std::cout);
        }
      }
    }
  }

  return 0;
}
