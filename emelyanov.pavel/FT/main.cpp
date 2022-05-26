#include <iostream>
#include <fstream>
#include <string>

#include "commands.h"
#include "messages.h"
#include "dataProcessing.h"

namespace {
  using DataSet = std::map< std::string, std::set< size_t > >;
  using AllDataSets = std::map< std::string, DataSet >;
  using CmdArgs = emelyanov::CommandArgs;

  const std::string ARGUMENT_ERROR = "The amount of arguments should be 2";
  const std::string FILE_IS_NOT_OPENED = "File isn't opened";
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << ARGUMENT_ERROR << '\n';
    return 1;
  }
  std::ofstream outputFile(argv[1]);
  if (!outputFile.is_open()) {
    std::cerr << FILE_IS_NOT_OPENED << '\n';
    return 1;
  }
  std::ostream& out = (argc == 2) ? outputFile : std::cout;
  AllDataSets allDataSets;
  while (!std::cin.eof()) {
    try {
      std::string commandToDo;
      std::getline(std::cin, commandToDo);
      if (!commandToDo.empty()) {
        CmdArgs commandLine = emelyanov::getArguments(commandToDo, ' ');
        emelyanov::Command command(allDataSets, std::cin, out);
        command(commandLine);
      }
    }
    catch (const std::exception& e) {
      emelyanov::printInvalidCommand(std::cerr) << '\n';
    }
  }
  return 0;
}
