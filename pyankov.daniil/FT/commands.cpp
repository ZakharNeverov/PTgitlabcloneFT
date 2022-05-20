#include "commands.hpp"
#include <iostream>
#include <stdexcept>
#include "support-operations.hpp"
#include "print-messages.hpp"
void pyankov::create(pyankov::matrices_t& matrices, std::istream& in)
{
  std::string type = "";
  std::string name = "";
  size_t rows = 0;
  size_t columns = 0;
  in >> type >> name >> rows >> columns;
  if (!in || !(type == "int" || type == "double"))
  {
    in.setstate(std::ios::failbit);
    throw std::invalid_argument("Incorrect matrix parameters!");
  }
  pyankov::Matrix< double > matrix(rows, columns, type);
  if (in.peek() != '\n')
  {
    in >> matrix;
  }
  if (in.peek() != '\n')
  {
    in.setstate(std::ios::failbit);
    throw std::invalid_argument("Incorrect input matrix!");
  }
  if (matrices.count(name))
  {
    in.setstate(std::ios::failbit);
    throw std::invalid_argument("Matrix already exist!");
  }
  matrices.insert({name, matrix});
}
void pyankov::multiply(pyankov::matrices_t& matrices, std::istream& in)
{
  std::string mainName = "";
  in >> mainName;
  std::vector< std::string > names;
  std::string type = "int";
  while (in.peek() != '\n')
  {
    std::string name = "";
    in >> name;
    if (!in || matrices.count(name) == 0)
    {
      in.setstate(std::ios::failbit);
      throw std::invalid_argument("Incorrect add command parameters!");
    }
    if (matrices.find(name)->second.getType() == "double")
    {
      type = "double";
    }
    names.push_back(name);
  }
  if (names.size() < 2)
  {
    in.setstate(std::ios::failbit);
    throw std::invalid_argument("Incorrect add command parameters!");
  }
  auto addMatrix = matrices.find(names.at(0))->second;
  pyankov::Matrix< double > mainMatrix(addMatrix.getRows(), addMatrix.getColumns(), type);
  mainMatrix.setZeroMatrix();
  mainMatrix += addMatrix;
  for (size_t i = 1; i < names.size(); i++)
  {
    addMatrix = matrices.find(names.at(i))->second;
    mainMatrix *= addMatrix;
  }
  if (matrices.count(mainName))
  {
    auto oldMatrix = matrices.find(mainName)->second;
    oldMatrix = mainMatrix;
    matrices.find(mainName)->second = oldMatrix;
  }
  else
  {
    matrices.insert({mainName, mainMatrix});
  }
}
void pyankov::add(pyankov::matrices_t& matrices, std::istream& in)
{
  std::string mainName = "";
  in >> mainName;
  std::vector< std::string > names;
  std::string type = "int";
  while (in.peek() != '\n')
  {
    std::string name = "";
    in >> name;
    if (!in || matrices.count(name) == 0)
    {
      in.setstate(std::ios::failbit);
      throw std::invalid_argument("Incorrect add command parameters!");
    }
    if (matrices.find(name)->second.getType() == "double")
    {
      type = "double";
    }
    names.push_back(name);
  }
  if (names.size() < 2)
  {
    in.setstate(std::ios::failbit);
    throw std::invalid_argument("Incorrect add command parameters!");
  }
  auto addMatrix = matrices.find(names.at(0))->second;
  pyankov::Matrix< double > mainMatrix(addMatrix.getRows(), addMatrix.getColumns(), type);
  mainMatrix.setZeroMatrix();
  mainMatrix += addMatrix;
  for (size_t i = 1; i < names.size(); i++)
  {
    addMatrix = matrices.find(names.at(i))->second;
    mainMatrix += addMatrix;
  }
  if (matrices.count(mainName))
  {
    auto oldMatrix = matrices.find(mainName)->second;
    oldMatrix = mainMatrix;
    matrices.find(mainName)->second = oldMatrix;
  }
  else
  {
    matrices.insert({mainName, mainMatrix});
  }
}
void pyankov::sub(pyankov::matrices_t& matrices, std::istream& in)
{
  std::string mainName = "";
  in >> mainName;
  std::vector< std::string > names;
  std::string type = "int";
  while (in.peek() != '\n')
  {
    std::string name = "";
    in >> name;
    if (!in || matrices.count(name) == 0)
    {
      in.setstate(std::ios::failbit);
      throw std::invalid_argument("Incorrect add command parameters!");
    }
    if (matrices.find(name)->second.getType() == "double")
    {
      type = "double";
    }
    names.push_back(name);
  }
  if (names.size() < 2)
  {
    in.setstate(std::ios::failbit);
    throw std::invalid_argument("Incorrect add command parameters!");
  }
  auto addMatrix = matrices.find(names.at(0))->second;
  pyankov::Matrix< double > mainMatrix(addMatrix.getRows(), addMatrix.getColumns(), type);
  mainMatrix.setZeroMatrix();
  mainMatrix += addMatrix;
  for (size_t i = 1; i < names.size(); i++)
  {
    addMatrix = matrices.find(names.at(i))->second;
    mainMatrix -= addMatrix;
  }
  if (matrices.count(mainName))
  {
    auto oldMatrix = matrices.find(mainName)->second;
    oldMatrix = mainMatrix;
    matrices.find(mainName)->second = oldMatrix;
  }
  else
  {
    matrices.insert({mainName, mainMatrix});
  }
}
void pyankov::concat(pyankov::matrices_t& matrices, std::istream& in)
{
  std::string direction = "";
  std::string resultName = "";
  std::string leftName = "";
  std::string rightName = "";
  in >> direction >> resultName >> leftName >> rightName;
  if (!in || in.peek() != '\n' || matrices.count(leftName) == 0 || matrices.count(rightName) == 0)
  {
    in.setstate(std::ios::failbit);
    throw std::invalid_argument("Incorrect concat parameters!");
  }
  auto leftMatrix = matrices.find(leftName)->second;
  auto rightMatrix = matrices.find(rightName)->second;
  try
  {
    if (direction == "toright")
    {
      pyankov::concatToRight(matrices, leftMatrix, rightMatrix, resultName);
    }
    else if (direction == "toleft")
    {
      pyankov::concatToRight(matrices, rightMatrix, leftMatrix, resultName);
    }
    else if (direction == "totop")
    {
      pyankov::concatToBot(matrices, rightMatrix, leftMatrix, resultName);
    }
    else if (direction == "tobot")
    {
      pyankov::concatToBot(matrices, leftMatrix, rightMatrix, resultName);
    }
    else
    {
      in.setstate(std::ios::failbit);
      throw std::invalid_argument("Incorrect direction!");
    }
  }
  catch (const std::exception& ex)
  {
    in.setstate(std::ios::failbit);
    throw std::invalid_argument("Incorrect concat arguments!");
  }
}
void pyankov::print(const pyankov::matrices_t& matrices, std::istream& in, std::ostream& out)
{
  std::string name = "";
  in >> name;
  if (!in || in.peek() != '\n' || matrices.count(name) == 0)
  {
    in.setstate(std::ios::failbit);
    throw std::invalid_argument("Incorrect print argument!");
  }
  out << matrices.find(name)->second;
}
void pyankov::equals(const pyankov::matrices_t& matrices, std::istream& in, std::ostream& out)
{
  std::string prevName = "";
  in >> prevName;
  if (!in || in.peek() == '\n' || matrices.count(prevName) == 0)
  {
    in.setstate(std::ios::failbit);
    throw std::invalid_argument("Incorrect matrix name!");
  }
  bool isEqual = true;
  while (in.peek() != '\n')
  {
    std::string currName = "";
    in >> currName;
    if (!in || matrices.count(currName) == 0)
    {
      in.setstate(std::ios::failbit);
      throw std::invalid_argument("Incorrect matrix name!");
    }
    if (!(matrices.find(prevName)->second == matrices.find(currName)->second))
    {
      isEqual = false;
    }
    prevName = currName;
  }
  if (isEqual)
  {
    pyankov::printTrue(out);
  }
  else
  {
    pyankov::printFalse(out);
  }
}
