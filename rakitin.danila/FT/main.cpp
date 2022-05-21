#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <string>

#include "dictionary_usings.h"
#include "dictionary_utils.h"
#include "io_commands_wrapper.h"

int main()
{
  rakitin::dictionaries data;

  using command = std::function< void(void) >;

  std::map< std::string, command > commands(
    { {"ADD", std::bind(&rakitin::executeAdd, std::ref(data), std::ref(std::cout), std::ref(std::cin))},
        {"FINDWORD", std::bind(&rakitin::executeFindWord, std::ref(data), std::ref(std::cout), std::ref(std::cin))},
        {"PRINTALL", std::bind(&rakitin::executePrintAll, std::ref(data), std::ref(std::cout), std::ref(std::cin))},
        {"MERGE", std::bind(&rakitin::executeMerge, std::ref(data), std::ref(std::cout), std::ref(std::cin))},
        {"CLEAR", std::bind(&rakitin::executeClear, std::ref(data), std::ref(std::cout), std::ref(std::cin))},
        {"INTERSECTION", std::bind(&rakitin::executeIntersection, std::ref(data), std::ref(std::cout), std::ref(std::cin))},
        {"SIMILARITY", std::bind(&rakitin::executeSimilarity, std::ref(data), std::ref(std::cout), std::ref(std::cin))},
        {"RENAME", std::bind(&rakitin::executeRename, std::ref(data), std::ref(std::cout), std::ref(std::cin))},
        {"DELETE", std::bind(&rakitin::executeDelete, std::ref(data), std::ref(std::cout), std::ref(std::cin))},
        {"PRINT_ALL_DATANAMES", std::bind(&rakitin::executePrintAllDatanames, std::ref(data), std::ref(std::cout))} });

  while (!std::cin.eof()) {
    try {
      std::cout << "Write command:\n";
      std::string command;
      std::cin >> command;
      auto cmd = commands.find(command);
      if (cmd != commands.end()) {
        cmd->second();
      }
      else if (command == "EXIT") {
        return 0;
      }
      throw std::logic_error("Unknown command");
    }
    catch (const std::exception& e) {
      std::cout << e.what() << "\n";
    }
  }
  return 0;
}
