#include "support-operations.hpp"
#include <stdexcept>
#include <vector>
void pyankov::concatToRight(matrices_t& matrices, const Matrix< double >& left, const Matrix< double >& right, const std::string& name)
{
  if (left.getRows() != right.getRows())
  {
    throw std::logic_error("Can not concat!");
  }
  std::vector< double > vector;
  for (size_t i = 0; i < left.getRows(); i++)
  {
    for (size_t j = 0; j < left.getColumns(); j++)
    {
      vector.push_back(left.getElement(i, j));
    }
    for (size_t j = 0; j < right.getColumns(); j++)
    {
      vector.push_back(right.getElement(i, j));
    }
  }
  std::string type = "int";
  if (left.getType() == "double" || right.getType() == "double")
  {
    type = "double";
  }
  pyankov::Matrix< double > result(left.getRows(), (left.getColumns() + right.getColumns()), vector, type);
  if (matrices.count(name))
  {
    auto matrix = matrices.find(name)->second;
    matrix = result;
    matrices.find(name)->second = result;
  }
  else
  {
    matrices.insert({name, result});
  }
}
void pyankov::concatToBot(matrices_t& matrices, const Matrix< double >& left, const Matrix< double >& right, const std::string& name)
{
  if (left.getColumns() != right.getColumns())
  {
    throw std::logic_error("Can not concat!");
  }
  std::vector< double > vector;
  for (size_t i = 0; i < left.getRows(); i++)
  {
    for (size_t j = 0; j < left.getColumns(); j++)
    {
      vector.push_back(left.getElement(i, j));
    }
  }
  for (size_t i = 0; i < right.getRows(); i++)
  {
    for (size_t j = 0; j < right.getColumns(); j++)
    {
      vector.push_back(right.getElement(i, j));
    }
  }
  std::string type = "int";
  if (left.getType() == "double" || right.getType() == "double")
  {
    type = "double";
  }
  pyankov::Matrix< double > result((left.getRows() + right.getRows()), left.getColumns(), vector, type);
  if (matrices.count(name))
  {
    auto matrix = matrices.find(name)->second;
    matrix = result;
    matrices.find(name)->second = result;
  }
  else
  {
    matrices.insert({name, result});
  }
}
