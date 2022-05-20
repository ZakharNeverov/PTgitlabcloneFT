#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include "iofmtguard.hpp"
namespace pyankov
{
  template< typename T >
  class Matrix
  {
  public:
    Matrix() = delete;
    Matrix(const Matrix< T >& other) = default;
    Matrix(Matrix< T >&& other) noexcept = default;
    Matrix(size_t rows, size_t columns, const std::string& type);
    Matrix(size_t rows, size_t columns, const std::vector< T >& vector, const std::string& type);
    ~Matrix() = default;
    Matrix< T >& operator=(const Matrix< T >& other) = default;
    Matrix< T >& operator=(Matrix< T >&& other) noexcept = default;
    Matrix< T >& operator+=(const Matrix< T >& other);
    Matrix< T >& operator-=(const Matrix< T >& other);
    Matrix< T >& operator*=(const Matrix< T >& other);
    size_t getRows() const;
    size_t getColumns() const;
    T getElement(size_t row, size_t column) const;
    std::string getType() const;
    void setZeroMatrix();
  private:
    size_t rows_;
    size_t columns_;
    std::vector< T > vector_;
    std::string type_;
  };
  template< typename T >
  bool operator==(const Matrix< T >& left, const Matrix< T >& right);
  template< typename T >
  std::istream& operator>>(std::istream& in, Matrix< T >& matrix);
  template< typename T >
  std::ostream& operator<<(std::ostream& out, const Matrix< T >& matrix);
}
template< typename T >
pyankov::Matrix< T >::Matrix(size_t rows, size_t columns, const std::string& type):
  rows_(rows),
  columns_(columns),
  vector_(),
  type_(type)
{
  bool isCorrectType = (type == "int") || (type == "double");
  if (rows == 0 || columns == 0 || !isCorrectType)
  {
    throw std::logic_error("Incorrect size!");
  }
  vector_.resize(rows_ * columns_);
  srand(time(0));
  size_t minValue = 10;
  size_t maxValue = 190;
  double multiplier = 10.0;
  for (size_t i = 0; i < rows_ * columns_; i++)
  {
    if (type == "int")
    {
      vector_.at(i) = static_cast< int >((rand() % maxValue + minValue) / multiplier);
    }
    else
    {
      vector_.at(i) = static_cast< double >((rand() % maxValue + minValue) / multiplier);
    }
  }
}
template< typename T >
pyankov::Matrix< T >::Matrix(size_t rows, size_t columns, const std::vector< T >& vector, const std::string& type):
  rows_(rows),
  columns_(columns),
  vector_(vector),
  type_(type)
{
  bool isCorrectType = (type == "int") || (type == "double");
  if (rows == 0 || columns == 0 || vector.size() != (rows * columns) || !isCorrectType)
  {
    throw std::logic_error("Incorrect matrix arguments!");
  }
}
template< typename T >
std::string pyankov::Matrix< T >::getType() const
{
  return type_;
}
template< typename T >
size_t pyankov::Matrix< T >::getRows() const
{
  return rows_;
}
template< typename T >
size_t pyankov::Matrix< T >::getColumns() const
{
  return columns_;
}
template< typename T >
T pyankov::Matrix< T >::getElement(size_t row, size_t column) const
{
  if (row > rows_ || column > columns_)
  {
    throw std::invalid_argument("Can not get the element with these indexes!");
  }
  return vector_.at(row * columns_ + column);
}
template< typename T >
void pyankov::Matrix< T >::setZeroMatrix()
{
  for (size_t i = 0; i < rows_ * columns_; i++)
  {
    vector_.at(i) = static_cast< T >(0);
  }
}
template< typename T >
pyankov::Matrix< T >& pyankov::Matrix< T >::operator+=(const pyankov::Matrix< T >& other)
{
  bool isSameSizes = (getRows() == other.getRows()) && (getColumns() == other.getColumns());
  if ((getType() == "int" && other.getType() == "double") || !isSameSizes)
  {
    throw std::logic_error("Can not add these matrices!");
  }
  std::vector< T > newVector(getRows() * getColumns());
  for (size_t i = 0; i < getRows(); i++)
  {
    for (size_t j = 0; j < getColumns(); j++)
    {
      newVector.at(i * getColumns() + j) = getElement(i, j) + other.getElement(i, j);
    }
  }
  pyankov::Matrix< T > newMatrix(getRows(), getColumns(), newVector, getType());
  *this = newMatrix;
  return *this;
}
template< typename T >
pyankov::Matrix< T >& pyankov::Matrix< T >::operator-=(const pyankov::Matrix< T >& other)
{
  bool isSameSizes = (getRows() == other.getRows()) && (getColumns() == other.getColumns());
  if ((getType() == "int" && other.getType() == "double") || !isSameSizes)
  {
    throw std::logic_error("Can not add these matrices!");
  }
  std::vector< T > newVector(getRows() * getColumns());
  for (size_t i = 0; i < getRows(); i++)
  {
    for (size_t j = 0; j < getColumns(); j++)
    {
      newVector.at(i * getColumns() + j) = getElement(i, j) - other.getElement(i, j);
    }
  }
  pyankov::Matrix< T > newMatrix(getRows(), getColumns(), newVector, getType());
  *this = newMatrix;
  return *this;
}
template< typename T >
pyankov::Matrix< T >& pyankov::Matrix< T >::operator*=(const pyankov::Matrix< T >& other)
{
  bool isCorrectSizes = getColumns() == other.getRows();
  bool isCorrectTypes = !(getType() == "int" && other.getType() == "double");
  if (!isCorrectSizes || !isCorrectTypes)
  {
    throw std::logic_error("Can not mulpiply these matrices!");
  }
  std::vector< T > vector(getRows() * other.getColumns());
  for (size_t i = 0; i < getRows(); i++)
  {
    for (size_t j = 0; j < other.getColumns(); j++)
    {
      for (size_t k = 0; k < getColumns(); k++)
      {
        vector.at(i * other.columns_ + j) += getElement(i, k) * other.getElement(k, j);
      }
    }
  }
  pyankov::Matrix< T > matrix(getRows(), other.getColumns(), vector, getType());
  *this = matrix;
  return *this;
}
template< typename T >
bool pyankov::operator==(const pyankov::Matrix< T >& left, const pyankov::Matrix< T >& right)
{
  if ((left.getRows() != right.getRows()) || (left.getColumns() != right.getColumns()))
  {
    return false;
  }
  for (size_t i = 0; i < left.getRows(); i++)
  {
    for (size_t j = 0; j < left.getColumns(); j++)
    {
      if (left.getElement(i, j) != right.getElement(i, j))
      {
        return false;
      }
    }
  }
  return true;
}
template< typename T >
std::istream& pyankov::operator>>(std::istream& in, pyankov::Matrix< T >& matrix)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::vector< T > vector(matrix.getRows() * matrix.getColumns());
  for (size_t i = 0; i < matrix.getRows() * matrix.getColumns(); i++)
  {
    in >> vector.at(i);
    if (in.peek() == '\n' && i != (matrix.getRows() * matrix.getColumns() - 1))
    {
      throw std::invalid_argument("Not enough numbers!");
    }
  }
  if (in)
  {
    matrix = pyankov::Matrix< T >(matrix.getRows(), matrix.getColumns(), vector, matrix.getType());
  }
  return in;
}
template< typename T >
std::ostream& pyankov::operator<<(std::ostream& out, const pyankov::Matrix< T >& matrix)
{
  pyankov::ioFmtGuard guard(out);
  out << "size: " << matrix.getRows() << "x" << matrix.getColumns() << "; type: " << matrix.getType() << "; elements\n";
  T maxElem = 0;
  for (size_t i = 0; i < matrix.getRows(); i++)
  {
    for (size_t j = 0; j < matrix.getColumns(); j++)
    {
      if (std::abs(matrix.getElement(i, j)) > maxElem)
      {
        maxElem = std::abs(matrix.getElement(i, j));
      }
    }
  }
  std::string str = std::to_string(maxElem);
  std::string substr = str.substr(0, str.find('.'));
  size_t addLen = 4;
  size_t maxLen = substr.length() + addLen;
  for (size_t i = 0; i < matrix.getRows(); i++)
  {
    out << std::left << std::setw(2) << '|';
    for (size_t j = 0; j < matrix.getColumns(); j++)
    {
      if (matrix.getType() == "int")
      {
        out << std::left << std::setw(maxLen) << (matrix.getElement(i, j));
      }
      else
      {
        out << std::fixed << std::left << std::setprecision(1) << std::setw(maxLen) << matrix.getElement(i, j);
      }
    }
    out << std::setw(1) << "|\n";
  }
  return out;
}
#endif
