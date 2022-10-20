#include <functional>
#include <iostream>
#include <limits>

#include "Graph.h"
#include "Messages.h"
#include "commands.h"

int main(int argv, char** argc)
{
  istaev::storage graphs;
  std::map< std::string, std::function< void(istaev::storage&) > > commands
  {
    {"create", istaev::Create(std::cin, std::cout)},
    {"print", istaev::PrintGraph(std::cin, std::cout)},
    {"add_vertex", istaev::AddVertex(std::cin, std::cout)},
    {"remove_vertex", istaev::RemoveVertex(std::cin, std::cout)},
    {"insert_edge", istaev::InsertEdge(std::cin, std::cout)},
    {"check_edge", istaev::CheckEdge(std::cin, std::cout)},
    {"bfs", istaev::Bfs(std::cin, std::cout)},
    {"calculate_diameter", istaev::CalcDiameter(std::cin, std::cout)}
  };
  std::string command = "";
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)(graphs);
    }
    catch (const std::exception& ex)
    {
      istaev::printInvalidCommand(std::cout) << "\n";
    }
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
