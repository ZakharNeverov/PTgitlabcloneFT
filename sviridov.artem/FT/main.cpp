#include <iostream>
#include <string>
#include <fstream>

#include "commands.hpp"
#include "dataProcessing.hpp"

namespace
{
  using MapOfGraphs = std::map < std::string, sviridov::MatrixGraph >;
  using CmdArgs = sviridov::CommandArgs;
}

int main()
{
  MapOfGraphs mapOfGraphs;

  std::string strWithCommand;
  while (std::getline(std::cin, strWithCommand))
  {
    try
    {
      if (!strWithCommand.empty())
      {
        CmdArgs arguments = sviridov::getArguments(strWithCommand, ' ');
        sviridov::Command command(mapOfGraphs, std::cout);
        command(arguments);
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what();
    }
  }
}
