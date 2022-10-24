#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include "Graph.hpp"
#include "commands.hpp"

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cerr << "Give me file pls" << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "File is not open" << std::endl;
    return 1;
  }

  try
  {
    funtov::Graph graph;
    file >> graph;
    std::map< std::string, std::function< void() > > shell = {
      {"JOINT", std::bind(&funtov::addJoit, std::ref(file), std::ref(graph))},
      {"APEX", std::bind(&funtov::addApex, std::ref(file), std::ref(graph))},
      {"DFS", std::bind(&funtov::doDfs, std::ref(graph))},
      {"DELETE", std::bind(&funtov::deleteGraph, std::ref(graph))},
      {"SORT", std::bind(&funtov::topoSort, std::ref(std::cout), std::ref(graph))},
      {"CHECK", std::bind(&funtov::checkGraph, std::ref(std::cout), std::ref(graph))},
      {"LOOK", std::bind(&funtov::lookWay, std::ref(file), std::ref(std::cout), std::ref(graph))},
      {"CHANGE", std::bind(&funtov::changeWay, std::ref(file), std::ref(graph))},
      {"REMOVE", std::bind(&funtov::deleteWay, std::ref(file), std::ref(graph))},
      {"OTHER", std::bind(&funtov::otherWaysFrom, std::ref(file), std::ref(std::cout), std::ref(graph))}};
    std::string cmd;
    file >> cmd;
    while (!file.eof())
    {
      shell[cmd]();
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
