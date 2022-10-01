#include "commands.hpp"
#include <iostream>
#include <stdexcept>
#include <functional>
bool pyankov::isContainingName(const pyankov::chain_t& chain, const std::string& name)
{
  return chain.first.count(name) || chain.second.count(name);
}
bool pyankov::compareMatricesByName(const pyankov::chain_t& chain, const std::string& left, const std::string& right)
{
  if (!(pyankov::isContainingName(chain, left) && pyankov::isContainingName(chain, right)))
  {
    throw std::invalid_argument("Can not find matrices with these names!");
  }
  if (chain.first.count(left) && chain.first.count(right))
  {
    return chain.first.find(left)->second == chain.first.find(right)->second;
  }
  else if (chain.first.count(left) && chain.second.count(right))
  {
    return chain.first.find(left)->second == chain.second.find(right)->second;
  }
  else if (chain.second.count(left) && chain.first.count(right))
  {
    return chain.second.find(left)->second == chain.first.find(right)->second;
  }
  else
  {
    return chain.second.find(left)->second == chain.second.find(right)->second;
  }
}
void pyankov::doCreateIntMatrix(pyankov::chain_t& chain, std::istream& in, int minValue, int maxValue, size_t precision)
{
  std::string name = "";
  size_t rows = 0;
  size_t columns = 0;
  in >> name >> rows >> columns;
  if (!in)
  {
    throw std::invalid_argument("Incorrect create parameters!");
  }
  pyankov::Matrix< int > matrix(rows, columns, minValue, maxValue, precision);
  if (in.peek() != '\n')
  {
    in >> matrix;
  }
  if (in.peek() != '\n' || pyankov::isContainingName(chain, name))
  {
    throw std::invalid_argument("Incorrect create parameters!");
  }
  chain.first.insert({name, matrix});
}
void pyankov::doCreateDoubleMatrix(pyankov::chain_t& chain, std::istream& in, int minValue, int maxValue, size_t precision)
{
  std::string name = "";
  size_t rows = 0;
  size_t columns = 0;
  in >> name >> rows >> columns;
  if (!in)
  {
    throw std::invalid_argument("Incorrect create parameters!");
  }
  pyankov::Matrix< double > matrix(rows, columns, minValue, maxValue, precision);
  if (in.peek() != '\n')
  {
    in >> matrix;
  }
  if (in.peek() != '\n' || pyankov::isContainingName(chain, name))
  {
    throw std::invalid_argument("Incorrect create parameters!");
  }
  chain.second.insert({name, matrix});
}
void pyankov::concatToRight(pyankov::chain_t& chain, const std::string& left, const std::string& right, const std::string& result)
{
  if (!pyankov::isContainingName(chain, left) || !pyankov::isContainingName(chain, right))
  {
    throw std::invalid_argument("Can not concat these matrices!");
  }
  bool isDoubleRes = chain.second.count(left) || chain.second.count(right);
  if (isDoubleRes)
  {
    pyankov::concatToRightDouble(chain, left, right, result);
  }
  else
  {
    pyankov::concatToRightInt(chain, left, right, result);
  }
}
void pyankov::concatToBot(pyankov::chain_t& chain, const std::string& left, const std::string& right, const std::string& result)
{
  if (!pyankov::isContainingName(chain, left) || !pyankov::isContainingName(chain, right))
  {
    throw std::invalid_argument("Can not concat these matrices!");
  }
  bool isDoubleRes = chain.second.count(left) || chain.second.count(right);
  if (isDoubleRes)
  {
    pyankov::concatToBotDouble(chain, left, right, result);
  }
  else
  {
    pyankov::concatToBotInt(chain, left, right, result);
  }
}
void pyankov::concatToRightInt(pyankov::chain_t& chain, const std::string& left, const std::string& right, const std::string& result)
{
  if (!chain.first.count(left) || !chain.first.count(right))
  {
    throw std::invalid_argument("It is not int matrices!");
  }
  pyankov::Matrix< int > leftMatrix = chain.first.find(left)->second;
  pyankov::Matrix< int > rightMatrix = chain.first.find(right)->second;
  if (leftMatrix.getRows() != rightMatrix.getRows())
  {
    throw std::logic_error("Can not concat these matrices!");
  }
  std::vector< int > vector;
  for (size_t i = 0; i < leftMatrix.getRows(); i++)
  {
    for (size_t j = 0; j < leftMatrix.getColumns(); j++)
    {
      vector.push_back(leftMatrix.getElement(i, j));
    }
    for (size_t j = 0; j < rightMatrix.getColumns(); j++)
    {
      vector.push_back(rightMatrix.getElement(i, j));
    }
  }
  pyankov::Matrix< int > resultMatrix(leftMatrix.getRows(), leftMatrix.getColumns() + rightMatrix.getColumns(), vector);
  if (chain.first.count(result) && !chain.second.count(result))
  {
    chain.first.find(result)->second = resultMatrix;
  }
  else if (!pyankov::isContainingName(chain, result))
  {
    chain.first.insert({result, resultMatrix});
  }
  else
  {
    throw std::logic_error("Incorrect concat arguments!");
  }
}
void pyankov::concatToBotInt(pyankov::chain_t& chain, const std::string& left, const std::string& right, const std::string& result)
{
  if (!chain.first.count(left) || !chain.first.count(right))
  {
    throw std::invalid_argument("It is not int matrices!");
  }
  pyankov::Matrix< int > leftMatrix = chain.first.find(left)->second;
  pyankov::Matrix< int > rightMatrix = chain.first.find(right)->second;
  if (leftMatrix.getColumns() != rightMatrix.getColumns())
  {
    throw std::logic_error("Can not concat these matrices!");
  }
  std::vector< int > vector;
  for (size_t i = 0; i < leftMatrix.getRows(); i++)
  {
    for (size_t j = 0; j < leftMatrix.getColumns(); j++)
    {
      vector.push_back(leftMatrix.getElement(i, j));
    }
  }
  for (size_t i = 0; i < rightMatrix.getRows(); i++)
  {
    for (size_t j = 0; j < rightMatrix.getColumns(); j++)
    {
      vector.push_back(rightMatrix.getElement(i, j));
    }
  }
  pyankov::Matrix< int > resultMatrix(leftMatrix.getRows() + leftMatrix.getRows(), leftMatrix.getColumns(), vector);
  if (chain.first.count(result) && !chain.second.count(result))
  {
    chain.first.find(result)->second = resultMatrix;
  }
  else if (!pyankov::isContainingName(chain, result))
  {
    chain.first.insert({result, resultMatrix});
  }
  else
  {
    throw std::logic_error("Incorrect concat arguments!");
  }
}
void pyankov::concatToRightDouble(pyankov::chain_t& chain, const std::string& left, const std::string& right, const std::string& result)
{
  bool isLeftInt = chain.first.count(left) != 0;
  bool isRightInt = chain.first.count(right) != 0;
  std::vector< double > vector;
  pyankov::Matrix< double > leftMatrix(1, 1);
  pyankov::Matrix< double > rightMatrix(1, 1);
  if (isLeftInt && !isRightInt)
  {
    pyankov::Matrix< int > leftInt = chain.first.find(left)->second;
    leftMatrix = leftInt.convertToDouble();
    rightMatrix = chain.second.find(right)->second;
  }
  else if (!isLeftInt && isRightInt)
  {
    leftMatrix = chain.second.find(left)->second;
    pyankov::Matrix< int > rightInt = chain.first.find(right)->second;
    rightMatrix = rightInt.convertToDouble();
  }
  else
  {
    leftMatrix = chain.second.find(left)->second;
    rightMatrix = chain.second.find(right)->second;
  }
  if (leftMatrix.getRows() != rightMatrix.getRows())
  {
    throw std::logic_error("Can not concat there matrices!");
  }
  for (size_t i = 0; i < leftMatrix.getRows(); i++)
  {
    for (size_t j = 0; j < leftMatrix.getColumns(); j++)
    {
      vector.push_back(leftMatrix.getElement(i, j));
    }
    for (size_t j = 0; j < rightMatrix.getColumns(); j++)
    {
      vector.push_back(rightMatrix.getElement(i, j));
    }
  }
  pyankov::Matrix< double > resultMatrix(leftMatrix.getRows(), leftMatrix.getColumns() + rightMatrix.getColumns(), vector);
  if (chain.second.count(result) && !chain.first.count(result))
  {
    chain.second.find(result)->second = resultMatrix;
  }
  else if (!pyankov::isContainingName(chain, result))
  {
    chain.second.insert({result, resultMatrix});
  }
  else
  {
    throw std::logic_error("Incorrect concat arguments!");
  }
}
void pyankov::concatToBotDouble(pyankov::chain_t& chain, const std::string& left, const std::string& right, const std::string& result)
{
  bool isLeftInt = chain.first.count(left) != 0;
  bool isRightInt = chain.first.count(right) != 0;
  std::vector< double > vector;
  pyankov::Matrix< double > leftMatrix(1, 1);
  pyankov::Matrix< double > rightMatrix(1, 1);
  if (isLeftInt && !isRightInt)
  {
    pyankov::Matrix< int > leftInt = chain.first.find(left)->second;
    leftMatrix = leftInt.convertToDouble();
    rightMatrix = chain.second.find(right)->second;
  }
  else if (!isLeftInt && isRightInt)
  {
    leftMatrix = chain.second.find(left)->second;
    pyankov::Matrix< int > rightInt = chain.first.find(right)->second;
    rightMatrix = rightInt.convertToDouble();
  }
  else
  {
    leftMatrix = chain.second.find(left)->second;
    rightMatrix = chain.second.find(right)->second;
  }
  if (leftMatrix.getColumns() != rightMatrix.getColumns())
  {
    throw std::logic_error("Can not concat these matrices!");
  }
  for (size_t i = 0; i < leftMatrix.getRows(); i++)
  {
    for (size_t j = 0; j < leftMatrix.getColumns(); j++)
    {
      vector.push_back(leftMatrix.getElement(i, j));
    }
  }
  for (size_t i = 0; i < rightMatrix.getRows(); i++)
  {
    for (size_t j = 0; j < rightMatrix.getColumns(); j++)
    {
      vector.push_back(rightMatrix.getElement(i, j));
    }
  }
  pyankov::Matrix< double > resultMatrix(leftMatrix.getRows() + rightMatrix.getRows(), leftMatrix.getColumns(), vector);
  if (chain.second.count(result) && !chain.first.count(result))
  {
    chain.second.find(result)->second = resultMatrix;
  }
  else if (!pyankov::isContainingName(chain, result))
  {
    chain.second.insert({result, resultMatrix});
  }
  else
  {
    throw std::logic_error("Incorrect concat arguments!");
  }
}
