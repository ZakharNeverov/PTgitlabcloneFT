#include "ioGraph.hpp"

#include <iostream>

std::istream& sviridov::operator>>(std::istream& is, MatrixGraph& matrixGraph)
{
  size_t rows = 0;
  is >> rows;
  size_t cols = 0;
  is >> cols;
  std::vector< std::vector< int > > matrix;
  for (size_t i = 0; i < rows; i++)
  {
    std::vector< int > temp;
    for (size_t j = 0; j < cols; j++)
    {
      int item = 0;
      is >> item;
      temp.push_back(item);
    }
    matrix.push_back(std::move(temp));
  }
  try
  {
    MatrixGraph tempGraph(matrix);
    matrixGraph = tempGraph;
  }
  catch (const std::exception&)
  {
    is.setstate(std::ios::failbit);
  }
  return is;
}

std::ostream& sviridov::operator<<(std::ostream& out, MatrixGraph& matrixGraph)
{
  for (size_t i = 0; i < matrixGraph.getMatrixSize(); i++)
  {
    for (size_t j = 0; j < matrixGraph.getMatrixSize(); j++)
    {
      out << matrixGraph.getData()[i][j] << ' ';
    }
    out << '\n';
  }
  return out;
}