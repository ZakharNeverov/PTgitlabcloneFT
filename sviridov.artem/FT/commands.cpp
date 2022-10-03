#include "commands.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>

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
    size_t rows = 0;
    fin >> rows;
    size_t cols = 0;
    fin >> cols;
    std::vector<std::vector<int>> matrix;
    for (size_t i = 0; i < rows; i++)
    {
      std::vector<int> temp;
      for (size_t j = 0; j < cols; j++)
      {
        int item = 0;
        fin >> item;
        temp.push_back(item);
      }
      matrix.push_back(temp);
    }
    MatrixGraph graph(matrix);
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
  try
  {
    mapOfGraphs_.at(nameOfGraph).print(out_);
  }
  catch (const std::out_of_range&)
  {
    throw std::logic_error("Error! No such graph!");
  }
  
}

void sviridov::Command::inVertexesDegrees(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  try
  {
    std::vector< int > inVertexesDegrees = mapOfGraphs_.at(nameOfGraph).inVertexesDegrees();
    for (size_t i = 0; i < inVertexesDegrees.size(); i++)
    {
      out_ << inVertexesDegrees[i] << ' ';
    }
  }
  catch (const std::out_of_range&)
  {
    throw std::logic_error("Error! No such graph!");
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
  try
  {
    std::vector< int > outVertexesDegrees = mapOfGraphs_.at(nameOfGraph).outVertexesDegrees();
    for (size_t i = 0; i < outVertexesDegrees.size(); i++)
    {
      out_ << outVertexesDegrees[i] << ' ';
    }
  }
  catch (const std::out_of_range&)
  {
    throw std::logic_error("Error! No such graph!");
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
  try
  {
    out_ << mapOfGraphs_.at(nameOfGraph).findVertexWithMaxDegree();
  }
  catch (const std::out_of_range&)
  {
    throw std::logic_error("Error! No such graph!");
  }
}

void sviridov::Command::removeMultipleArcs(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  try
  {
    MatrixGraph currentGraph = mapOfGraphs_.at(nameOfGraph);
    currentGraph.removeMultipleArcs();
    currentGraph.print(out_);
  }
  catch (const std::out_of_range&)
  {
    throw std::logic_error("Error! No such graph!");
  }
}

void sviridov::Command::removeCycles(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  try
  {
    MatrixGraph currentGraph = mapOfGraphs_.at(nameOfGraph);
    bool onlyHamiltonian = false;
    currentGraph.removeCycles(onlyHamiltonian);
    currentGraph.print(out_);
  }
  catch (const std::out_of_range&)
  {
    throw std::logic_error("Error! No such graph!");
  }
}

void sviridov::Command::removeHamiltonianCycles(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  try
  {
    MatrixGraph currentGraph = mapOfGraphs_.at(nameOfGraph);
    bool onlyHamiltonian = true;
    currentGraph.removeCycles(onlyHamiltonian);
    currentGraph.print(out_);
  }
  catch (const std::out_of_range&)
  {
    throw std::logic_error("Error! No such graph!");
  }
}

void sviridov::Command::sourceVertexes(CommandArgs& args)
{
  if (args.size() != 1)
  {
    throw std::logic_error("Error! Number of arguments is invalid!");
  }
  std::string nameOfGraph = args.front();
  args.pop_front();
  try
  {
    std::vector< size_t > sourceVertexes = mapOfGraphs_.at(nameOfGraph).findSourceVertexes();
    for (size_t i = 0; i < sourceVertexes.size(); i++)
    {
      out_ << sourceVertexes[i] << ' ';
    }
  }
  catch (const std::out_of_range&)
  {
    throw std::logic_error("Error! No such graph!");
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
  try
  {
    std::vector< size_t > sinkVertexes = mapOfGraphs_.at(nameOfGraph).findSinkVertexes();
    for (size_t i = 0; i < sinkVertexes.size(); i++)
    {
      out_ << sinkVertexes[i] << ' ';
    }
  }
  catch (const std::out_of_range&)
  {
    throw std::logic_error("Error! No such graph!");
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
  size_t vertexNumber = 0;
  if (!getNumber(strVertexNumber, vertexNumber))
  {
    throw std::invalid_argument("Error! Invalid vertex number!");
  }
  try
  {
    std::vector< size_t > adjecentVertexes = mapOfGraphs_.at(nameOfGraph).findAdjecentVertexes(vertexNumber);
    for (size_t i = 0; i < adjecentVertexes.size(); i++)
    {
      out_ << adjecentVertexes[i] << ' ';
    }
  }
  catch (const std::out_of_range&)
  {
    throw std::logic_error("Error! No such graph!");
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
  size_t vertexFrom = 0;
  size_t vertexTo = 0;
  if (!getNumber(strVertexFrom, vertexFrom))
  {
    throw std::invalid_argument("Error! Invalid vertex number!");
  }
  if (!getNumber(strVertexTo, vertexTo))
  {
    throw std::invalid_argument("Error! Invalid vertex number!");
  }
  try
  {
    std::vector< size_t > path = mapOfGraphs_.at(nameOfGraph).findPath(vertexFrom, vertexTo);
    for (size_t i = 0; i < path.size(); i++)
    {
      out_ << path[i] << ' ';
    }
  }
  catch (const std::out_of_range&)
  {
    throw std::logic_error("Error! No such graph!");
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
  try
  {
    mapOfGraphs_.at(nameOfGraph).print(fout);
  }
  catch (const std::out_of_range&)
  {
    throw std::logic_error("Error! No such graph!");
  }
}

bool sviridov::getNumber(std::string str, size_t number)
{
  if (str.find_first_not_of("0123456789") != str.npos)
  {
    return false;
  }
  number = std::stoi(str);
  return true;
}