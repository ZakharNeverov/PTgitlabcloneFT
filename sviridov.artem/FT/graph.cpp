#include "graph.hpp"

#include <stdexcept>

sviridov::MatrixGraph::MatrixGraph(std::vector<std::vector<int>>& matrixGraph):
  matrixGraph(matrixGraph)
{
  for (size_t row = 0; row < matrixGraph.size(); row++)
  {
    if (matrixGraph[row].size() != matrixGraph.size())
    {
      throw std::invalid_argument("Error! Matrix is not correct!");
    }
    for (size_t col = 0; col < matrixGraph.size(); col++)
    {
      if (matrixGraph[row][col] < 0)
      {
        throw std::invalid_argument("Error! Matrix is not correct!");
      }
    }
  }
}

bool sviridov::MatrixGraph::hasVertex(size_t vertex)
{
  if (this->matrixGraph.empty() || vertex > this->matrixGraph.size() - 1)
  {
    return false;
  }
  return true;
}

bool sviridov::MatrixGraph::hasArc(size_t vertexFrom, size_t vertexTo)
{
  if (this->matrixGraph.empty())
  {
    throw std::logic_error("Error! Graph is empty!");
  }
  if (!hasVertex(vertexFrom) || !hasVertex(vertexTo))
  {
    throw std::invalid_argument("Error! Invalid arc!");
  }
  return matrixGraph[vertexFrom][vertexTo] != 0;
}

void sviridov::MatrixGraph::removeArc(size_t vertexFrom, size_t vertexTo)
{
  if (this->matrixGraph.empty())
  {
    throw std::logic_error("Error! Graph is empty!");
  }
  if (!hasVertex(vertexFrom) || !hasVertex(vertexTo))
  {
    throw std::invalid_argument("Error! Invalid arc!");
  }
  matrixGraph[vertexFrom][vertexTo] = 0;
}

std::vector<int> sviridov::MatrixGraph::inVertexesDegrees()
{
  std::vector<int> out(matrixGraph.size(), 0);
  for (size_t col = 0; col < matrixGraph.size(); col++)
  {
    out[col] += inVertexDegree(col);
  }
  return out;
}

std::vector<int> sviridov::MatrixGraph::outVertexesDegrees()
{
  std::vector<int> out(matrixGraph.size(), 0);
  for (size_t row = 0; row < matrixGraph.size(); row++)
  {
    out[row] += outVertexDegree(row);
  }
  return out;
}

size_t sviridov::MatrixGraph::findVertexWithMaxDegree()
{
  int maxDegree = 0;
  int maxVertex = 0;
  for (size_t vertex = 0; vertex < matrixGraph.size(); vertex++)
  {
    int degree = inVertexDegree(vertex) + outVertexDegree(vertex);
    if (degree > maxDegree)
    {
      maxDegree = degree;
      maxVertex = vertex;
    }
  }
  return maxVertex;
}

void sviridov::MatrixGraph::removeMultipleArcs()
{
  for (size_t row = 0; row < matrixGraph.size(); row++)
  {
    for (size_t col = 0; col < matrixGraph.size(); col++)
    {
      if (matrixGraph[row][col] > 1)
      {
        matrixGraph[row][col] = 1;
      }
    }
  }
}

std::vector<size_t> sviridov::MatrixGraph::findSourceVertexes()
{
  std::vector<size_t> out(0);
  for (size_t vertex = 0; vertex < matrixGraph.size(); vertex++)
  {
    if (inVertexDegree(vertex) == 0)
    {
      out.push_back(vertex);
    }
  }
  return out;
}

std::vector<size_t> sviridov::MatrixGraph::findSinkVertexes()
{
  std::vector<size_t> out(0);
  for (size_t vertex = 0; vertex < matrixGraph.size(); vertex++)
  {

    if (outVertexDegree(vertex) == 0)
    {
      out.push_back(vertex);
    }
  }
  return out;
}

std::vector<size_t> sviridov::MatrixGraph::findAdjecentVertexes(size_t vertex)
{
  if (!hasVertex(vertex))
  {
    throw std::invalid_argument("Error! Invalid vertex number!");
  }
  std::vector<size_t> out;
  for (size_t col = 0; col < matrixGraph.size(); col++)
  {
    if (matrixGraph[vertex][col] != 0)
    {
      out.push_back(col);
    }
  }
  return out;
}

std::vector<size_t> sviridov::MatrixGraph::findPath(size_t from, size_t to)
{
  if (!hasVertex(from) || !hasVertex(to))
  {
    throw std::invalid_argument("Error! Invalid vertex number!");
  }
  std::vector<size_t> out;
  std::vector<std::string> colors(matrixGraph.size(), "white");
  doFindingPath(from, to, out, colors);
  return out;
}

void sviridov::MatrixGraph::doFindingPath(size_t row, const size_t to, std::vector<size_t>& out, std::vector<std::string>& colors)
{
  colors[row] = "grey";
  out.push_back(row);
  if (!matrixGraph[row][to])
  {
    for (size_t col = 0; col < matrixGraph.size(); col++)
    {
      if (matrixGraph[row][col] != 0 && colors[col] == "white")
      {
        doFindingPath(col, to, out, colors);
      }
      else if (col == matrixGraph.size() - 1)
      {
        throw std::logic_error("Error! No path");
      }
    }
  }
}

void sviridov::MatrixGraph::removeCycles(bool onlyHamiltonian)
{
  if (this->matrixGraph.empty())
  {
    throw std::invalid_argument("Error! Graph is empty!");
  }
  std::vector<std::string> colors(matrixGraph.size(), "white");
  if (onlyHamiltonian)
  {
    doRemovingHamiltonianCycles(0, colors);
  }
  else
  {
    doRemovingCycles(0, colors);
  }
}

void sviridov::MatrixGraph::doRemovingCycles(size_t row, std::vector<std::string>& colors)
{
  colors[row] = "grey";
  for (size_t col = 0; col < matrixGraph.size(); col++)
  {
    if (matrixGraph[row][col] != 0 && colors[col] == "white")
    {
      doRemovingCycles(col, colors);
    }
    else if (matrixGraph[row][col] != 0 && colors[col] == "grey")
    {
      removeArc(row, col);
    }
    if (col == matrixGraph.size() - 1)
    {
      colors[row] = "black";
      if (row != matrixGraph.size() - 1)
      {
        doRemovingCycles(row + 1, colors);
      }
    }
  }
}

void sviridov::MatrixGraph::doRemovingHamiltonianCycles(size_t row, std::vector<std::string>& colors)
{
  colors[row] = "grey";
  for (size_t col = 0; col < matrixGraph.size(); col++)
  {
    if (matrixGraph[row][col] != 0 && colors[col] == "white")
    {
      doRemovingHamiltonianCycles(col, colors);
    }
    else if (matrixGraph[row][col] != 0 && colors[col] == "grey" 
      && std::equal(colors.begin() + 1, colors.end(), colors.begin()))
    {
      removeArc(row, col);
    }
    if (col == matrixGraph.size() - 1)
    {
      colors[row] = "black";
      if (row != matrixGraph.size() - 1)
      {
        doRemovingHamiltonianCycles(row + 1, colors);
      }
    }
  }
}

int sviridov::MatrixGraph::inVertexDegree(size_t vertex)
{
  if (!hasVertex(vertex))
  {
    throw std::invalid_argument("Error! Invalid vertex number!");
  }
  int count = 0;
  for (size_t row = 0; row < matrixGraph.size(); row++)
  {
    count += matrixGraph[row][vertex];
  }
  return count;
}

int sviridov::MatrixGraph::outVertexDegree(size_t vertex)
{
  if (!hasVertex(vertex))
  {
    throw std::invalid_argument("Error! Invalid vertex number!");
  }
  int count = 0;
  for (size_t col = 0; col < matrixGraph.size(); col++)
  {
    count += matrixGraph[vertex][col];
  }
  return count;
}

void sviridov::MatrixGraph::print(std::ostream& out)
{
  for (size_t i = 0; i < matrixGraph.size(); i++)
  {
    for (size_t j = 0; j < matrixGraph.size(); j++)
    {
      out << matrixGraph[i][j] << ' ';
    }
    out << '\n';
  }
}
