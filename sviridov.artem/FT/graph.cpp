#include "graph.hpp"

#include <stdexcept>

sviridov::MatrixGraph::MatrixGraph(std::vector<std::vector<int>>& matrixGraph):
  matrixGraph_(matrixGraph)
{
  for (size_t row = 0; row < matrixGraph_.size(); row++)
  {
    if (matrixGraph_[row].size() != matrixGraph_.size())
    {
      throw std::invalid_argument("Error! Matrix is not correct!");
    }
    for (size_t col = 0; col < matrixGraph_.size(); col++)
    {
      if (matrixGraph_[row][col] < 0)
      {
        throw std::invalid_argument("Error! Matrix is not correct!");
      }
    }
  }
}

bool sviridov::MatrixGraph::hasVertex(size_t vertex)
{
  if (this->matrixGraph_.empty() || vertex > this->matrixGraph_.size() - 1)
  {
    return false;
  }
  return true;
}

bool sviridov::MatrixGraph::hasArc(size_t vertexFrom, size_t vertexTo)
{
  if (this->matrixGraph_.empty())
  {
    throw std::logic_error("Error! Graph is empty!");
  }
  if (!hasVertex(vertexFrom) || !hasVertex(vertexTo))
  {
    throw std::invalid_argument("Error! Invalid arc!");
  }
  return matrixGraph_[vertexFrom][vertexTo] != 0;
}

void sviridov::MatrixGraph::removeArc(size_t vertexFrom, size_t vertexTo)
{
  if (this->matrixGraph_.empty())
  {
    throw std::logic_error("Error! Graph is empty!");
  }
  if (!hasVertex(vertexFrom) || !hasVertex(vertexTo))
  {
    throw std::invalid_argument("Error! Invalid arc!");
  }
  matrixGraph_[vertexFrom][vertexTo] = 0;
}

std::vector<int> sviridov::MatrixGraph::inVertexesDegrees()
{
  std::vector<int> out(matrixGraph_.size(), 0);
  for (size_t col = 0; col < matrixGraph_.size(); col++)
  {
    out[col] += inVertexDegree(col);
  }
  return out;
}

std::vector<int> sviridov::MatrixGraph::outVertexesDegrees()
{
  std::vector<int> out(matrixGraph_.size(), 0);
  for (size_t row = 0; row < matrixGraph_.size(); row++)
  {
    out[row] += outVertexDegree(row);
  }
  return out;
}

size_t sviridov::MatrixGraph::findVertexWithMaxDegree()
{
  int maxDegree = 0;
  int maxVertex = 0;
  for (size_t vertex = 0; vertex < matrixGraph_.size(); vertex++)
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
  for (size_t row = 0; row < matrixGraph_.size(); row++)
  {
    for (size_t col = 0; col < matrixGraph_.size(); col++)
    {
      if (matrixGraph_[row][col] > 1)
      {
        matrixGraph_[row][col] = 1;
      }
    }
  }
}

std::vector<size_t> sviridov::MatrixGraph::findSourceVertexes()
{
  std::vector<size_t> out(0);
  for (size_t vertex = 0; vertex < matrixGraph_.size(); vertex++)
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
  for (size_t vertex = 0; vertex < matrixGraph_.size(); vertex++)
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
  for (size_t col = 0; col < matrixGraph_.size(); col++)
  {
    if (matrixGraph_[vertex][col] != 0)
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
  std::vector<std::string> colors(matrixGraph_.size(), "white");
  doFindingPath(from, to, out, colors);
  return out;
}

void sviridov::MatrixGraph::doFindingPath(size_t row, const size_t to, std::vector<size_t>& out, std::vector<std::string>& colors)
{
  colors[row] = "grey";
  out.push_back(row);
  if (!matrixGraph_[row][to])
  {
    for (size_t col = 0; col < matrixGraph_.size(); col++)
    {
      if (matrixGraph_[row][col] != 0 && colors[col] == "white")
      {
        doFindingPath(col, to, out, colors);
      }
      else if (col == matrixGraph_.size() - 1)
      {
        throw std::logic_error("Error! No path");
      }
    }
  }
}

void sviridov::MatrixGraph::removeCycles(bool onlyHamiltonian)
{
  if (this->matrixGraph_.empty())
  {
    throw std::invalid_argument("Error! Graph is empty!");
  }
  std::vector<std::string> colors(matrixGraph_.size(), "white");
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
  for (size_t col = 0; col < matrixGraph_.size(); col++)
  {
    if (matrixGraph_[row][col] != 0 && colors[col] == "white")
    {
      doRemovingCycles(col, colors);
    }
    else if (matrixGraph_[row][col] != 0 && colors[col] == "grey")
    {
      removeArc(row, col);
    }
    if (col == matrixGraph_.size() - 1)
    {
      colors[row] = "black";
      if (row != matrixGraph_.size() - 1)
      {
        doRemovingCycles(row + 1, colors);
      }
    }
  }
}

void sviridov::MatrixGraph::doRemovingHamiltonianCycles(size_t row, std::vector<std::string>& colors)
{
  colors[row] = "grey";
  for (size_t col = 0; col < matrixGraph_.size(); col++)
  {
    if (matrixGraph_[row][col] != 0 && colors[col] == "white")
    {
      doRemovingHamiltonianCycles(col, colors);
    }
    else if (matrixGraph_[row][col] != 0 && colors[col] == "grey" 
      && std::equal(colors.begin() + 1, colors.end(), colors.begin()))
    {
      removeArc(row, col);
    }
    if (col == matrixGraph_.size() - 1)
    {
      colors[row] = "black";
      if (row != matrixGraph_.size() - 1)
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
  for (size_t row = 0; row < matrixGraph_.size(); row++)
  {
    count += matrixGraph_[row][vertex];
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
  for (size_t col = 0; col < matrixGraph_.size(); col++)
  {
    count += matrixGraph_[vertex][col];
  }
  return count;
}

void sviridov::MatrixGraph::print(std::ostream& out)
{
  for (size_t i = 0; i < matrixGraph_.size(); i++)
  {
    for (size_t j = 0; j < matrixGraph_.size(); j++)
    {
      out << matrixGraph_[i][j] << ' ';
    }
    out << '\n';
  }
}
