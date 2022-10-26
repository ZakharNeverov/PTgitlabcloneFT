#include "commands.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>

#include "ioGraph.hpp"

sviridov::Command::Command(MapOfGraphs& mapOfGraphs, std::ostream& out):
  mapOfGraphs_(mapOfGraphs),
  out_(out),
  mapOfCommands_(
    {
      {"getGraphs", std::bind(&sviridov::Command::getGraphs, this, std::placeholders::_1)},
      {"print", std::bind(&sviridov::Command::print, this, std::placeholders::_1)},
      {"inVertexesDegrees", std::bind(&sviridov::Command::inVertexesDegrees, this, std::placeholders::_1)},
      {"outVertexesDegrees", std::bind(&sviridov::Command::outVertexesDegrees, this, std::placeholders::_1)},
      {"vertexWithMaxDegree", std::bind(&sviridov::Command::vertexWithMaxDegree, this, std::placeholders::_1)},
      {"removeMultipleArcs", std::bind(&sviridov::Command::removeMultipleArcs, this, std::placeholders::_1)},
      {"removeCycles", std::bind(&sviridov::Command::removeCycles, this, std::placeholders::_1)},
      {"removeHamiltonianCycles", std::bind(&sviridov::Command::removeHamiltonianCycles, this, std::placeholders::_1)},
      {"sourceVertexes", std::bind(&sviridov::Command::sourceVertexes, this, std::placeholders::_1)},
      {"sinkVertexes", std::bind(&sviridov::Command::sinkVertexes, this, std::placeholders::_1)},
      {"adjacentVertexes", std::bind(&sviridov::Command::adjacentVertexes, this, std::placeholders::_1)},
      {"path", std::bind(&sviridov::Command::path, this, std::placeholders::_1)},
      {"saveGraph", std::bind(&sviridov::Command::saveGraph, this, std::placeholders::_1)}
    })
{}

void sviridov::Command::operator()(CommandArgs& args)
{
  std::string command = args.front();
  args.pop_front();
  try
  {
    mapOfCommands_.at(command)(args);
  }
  catch (const std::exception&)
  {
    throw std::logic_error("Error! Unexpected command!");
  }
}

void sviridov::Command::getGraphs(CommandArgs& args)
{
  if (args.size() != 3)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string fileName = args.front();
  args.pop_front();
  std::ifstream fin;
  fin.open(fileName);
  if (!fin.is_open())
  {
    throw std::logic_error("Error! File is not opened!");
  }
  while (!fin.eof())
  {
    std::string graphName;
    std::getline(fin, graphName);
    MatrixGraph graph;
    fin >> graph;
    mapOfGraphs_.insert(std::make_pair(graphName, graph));
  }
}

void sviridov::Command::print(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  out_ << mapOfGraphs_.at(nameOfGraph);
}

void sviridov::Command::inVertexesDegrees(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  std::vector< int > inVertexesDegrees = mapOfGraphs_.at(nameOfGraph).inVertexesDegrees();
  for (size_t i = 0; i < inVertexesDegrees.size(); i++)
  {
    out_ << inVertexesDegrees[i] << ' ';
  }
}

void sviridov::Command::outVertexesDegrees(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  std::vector< int > outVertexesDegrees = mapOfGraphs_.at(nameOfGraph).outVertexesDegrees();
  for (size_t i = 0; i < outVertexesDegrees.size(); i++)
  {
    out_ << outVertexesDegrees[i] << ' ';
  }
}

void sviridov::Command::vertexWithMaxDegree(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  out_ << mapOfGraphs_.at(nameOfGraph).findVertexWithMaxDegree();
}

void sviridov::Command::removeMultipleArcs(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  MatrixGraph currentGraph = mapOfGraphs_.at(nameOfGraph);
  currentGraph.removeMultipleArcs();
  out_ << currentGraph;
}

void sviridov::Command::removeCycles(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  MatrixGraph currentGraph = mapOfGraphs_.at(nameOfGraph);
  bool onlyHamiltonian = false;
  currentGraph.removeCycles(onlyHamiltonian);
  out_ << currentGraph;
}

void sviridov::Command::removeHamiltonianCycles(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  MatrixGraph currentGraph = mapOfGraphs_.at(nameOfGraph);
  bool onlyHamiltonian = true;
  currentGraph.removeCycles(onlyHamiltonian);
  out_ << currentGraph;
}

void sviridov::Command::sourceVertexes(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  std::vector< size_t > sourceVertexes = mapOfGraphs_.at(nameOfGraph).findSourceVertexes();
  for (size_t i = 0; i < sourceVertexes.size(); i++)
  {
    out_ << sourceVertexes[i] << ' ';
  }
}

void sviridov::Command::sinkVertexes(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  std::vector< size_t > sinkVertexes = mapOfGraphs_.at(nameOfGraph).findSinkVertexes();
  for (size_t i = 0; i < sinkVertexes.size(); i++)
  {
    out_ << sinkVertexes[i] << ' ';
  }
}

void sviridov::Command::adjacentVertexes(CommandArgs& args)
{
  if (args.size() != 2)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  std::string strVertexNumber = args.front();
  args.pop_front();
  size_t vertexNumber = std::stoi(strVertexNumber);
  std::vector< size_t > adjecentVertexes = mapOfGraphs_.at(nameOfGraph).findAdjecentVertexes(vertexNumber);
  for (size_t i = 0; i < adjecentVertexes.size(); i++)
  {
    out_ << adjecentVertexes[i] << ' ';
  }
}

void sviridov::Command::path(CommandArgs& args)
{
  if (args.size() != 3)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  std::string strVertexFrom = args.front();
  args.pop_front();
  std::string strVertexTo = args.front();
  args.pop_front();
  size_t vertexFrom = std::stoi(strVertexFrom);
  size_t vertexTo = std::stoi(strVertexTo);
  std::vector< size_t > path = mapOfGraphs_.at(nameOfGraph).findPath(vertexFrom, vertexTo);
  for (size_t i = 0; i < path.size(); i++)
  {
    out_ << path[i] << ' ';
  }
}

void sviridov::Command::saveGraph(CommandArgs& args)
{
  std::string nameOfGraph = args.front();
  args.pop_front();
  std::string nameOfFile = args.front();
  args.pop_front();
  std::ofstream fout;
  fout.open(nameOfFile);
  if (!fout.is_open())
  {
    throw std::logic_error("Error! File is not opened!");
  }
  fout << mapOfGraphs_.at(nameOfGraph);
}
