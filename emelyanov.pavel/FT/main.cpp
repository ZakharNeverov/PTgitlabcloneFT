#include <iostream>

#include "commands.h"
#include "messages.h"
#include "dataProcessing.h"

namespace {
  using DataSet = std::map< std::string, std::set< size_t > >;
  using AllDataSets = std::map< std::string, DataSet >;
  using CmdArgs = emelyanov::CommandArgs;
}

int main()
{
  AllDataSets allDataSets;
  while (!std::cin.eof()) {
    try {
      std::string commandToDo;
      std::getline(std::cin, commandToDo);
      if (!commandToDo.empty()) {
        CmdArgs commandLine = emelyanov::getArguments(commandToDo, ' ');
        emelyanov::Command command(allDataSets, std::cin, std::cout);
        command(commandLine);
      }
    }
    catch (const std::exception& e) {
      emelyanov::printInvalidCommand(std::cout) << '\n';
    }
  }
  return 0;
}
