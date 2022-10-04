#include <iostream>
#include <string>
#include <list>
#include "graph.hpp"
#include "dataProcessing.h"
#include "commands.h"

namespace {
  using CmdArgs = std::list< std::string >;
  using AllGraphs = std::map< std::string, mitkov::orientedGraph< int > >;
}

int main()
{
  AllGraphs allGraphs;
  while (!std::cin.eof()) {
    try {
      std::string commandToDo;
      std::getline(std::cin, commandToDo);
      if (!commandToDo.empty()) {
        CmdArgs commandLine = mitkov::getArguments(commandToDo, ' ');
        mitkov::Command command(allGraphs, std::cout);
        command(commandLine);
      }
    }
    catch (const std::exception& e) {
      std::cerr << e.what();
    }
  }
  return 0;
}
