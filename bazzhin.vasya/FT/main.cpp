#include <iostream>
#include <fstream>
#include <functional>
#include <limits>

#include "reading.hpp"
#include "commands.hpp"
#include "messages.hpp"

int main(int argv, char** argc)
{
  bazzhin::dictionaries dicts;
  std::ifstream file;
  if (argv >= 2)
  {
    for (int i = 1; i < argv; ++i)
    {
      std::string name = std::string(argc[i]);
      file.open(argc[i]);
      if (file.is_open())
      {
        dicts.insert(std::make_pair(name.substr(0, name.find('.')), bazzhin::readDictionary(file)));
      }
      else
      {
        std::cerr << "It is impossible to create dictionary, file is not open! ";
      }
      file.close();
    }
  }

  bazzhin::Commands commands(std::cin, std::cout, dicts);
  std::map< std::string, std::function< void(void) > > commandStorage
  {
    {"CREATE_DICTIONARY", [&commands] { commands.createDictionary(); }},
    {"DELETE_DICTIONARY", [&commands] { commands.deleteDictionary(); }},
    {"PRINT_DICTIONARY", [&commands] { commands.printDictionary(); }},
    {"INSERT_WORD", [&commands] { commands.insertWord(); }},
    {"SEARCH_WORD", [&commands] { commands.searchWordInDictionary(); }},
    {"DELETE_WORD", [&commands] { commands.deleteWordInDictionary(); }},
    {"DELETE_WORD_ALL", [&commands] { commands.deleteWordInAllDictionaries(); }},
    {"PRINT_WORDS", [&commands] { commands.printAllWords(); }},
    {"ADD_WORDS_FROM_TEXT", [&commands] { commands.addWordsFromText(); }},
    {"PRINT_PERIOD", [&commands] { commands.printPeriodOfWord(); }},
    {"PRINT_MAIN_WORDS", [&commands] { commands.printMainWords(); }}
  };

  std::string command = "";
  while (std::cin >> command)
  {
    try
    {
      commandStorage.at(command)();
    }
    catch (const std::exception& ex)
    {
      bazzhin::printInvalidCommand(std::cout) << "\n";
    }
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
