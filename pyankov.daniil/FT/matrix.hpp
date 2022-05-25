#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <random>
#include <vector>
#include <limits>
#include <type_traits>
#include <cstdlib>
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
    Matrix(size_t rows, size_t columns);
    Matrix(size_t rows, size_t columns, const std::vector< T >& vector);
    ~Matrix() = default;
    Matrix< T >& operator=(const Matrix< T >& other) = default;
    Matrix< T >& operator=(Matrix< T >&& other) noexcept = default;
    size_t getRows() const;
    size_t getColumns() const;
    T getElement(size_t row, size_t column) const;
    size_t getPrecision() const;
    void setElement(size_t row, size_t column, T element);
    void setZeroMatrix();
    static void setStaticValues(int minValue, int maxValue, size_t precision);
    Matrix< double > convertToDouble();
  private:
    static int minValue_;
    static int maxValue_;
    static size_t precision_;
    size_t rows_;
    size_t columns_;
    std::vector< T > vector_;
  };
  template< typename T, typename S >
  Matrix< T >& operator+=(Matrix< T >& left, const Matrix< S >& right);
  template< typename T, typename S >
  Matrix< T >& operator-=(Matrix< T >& left, const Matrix< S >& right);
  template< typename T, typename S >
  Matrix< T >& operator*=(Matrix< T >& left, const Matrix< S >& right);
  template< typename T, typename S >
  bool operator==(const Matrix< T >& left, const Matrix< S >& right);
  template< typename T >
  std::istream& operator>>(std::istream& in, Matrix< T >& matrix);
  template< typename T >
  std::ostream& operator<<(std::ostream& out, const Matrix< T >& matrix);
}
template< typename T >
pyankov::Matrix< T >::Matrix(size_t rows, size_t columns):
  rows_(rows),
  columns_(columns),
  vector_()
{
  if (rows == 0 || columns == 0)
  {
    throw std::logic_error("Incorrect matrix size!");
  }
  vector_.resize(rows_ * columns_);
  int newMaxValue = maxValue_ * std::pow(10, precision_);
  int newMinValue = minValue_ * std::pow(10, precision_);
  double multiplier = std::pow(10, precision_);
  for (size_t i = 0; i < rows_ * columns_; i++)
  {
    if (std::is_same< T, int >::value)
    {
      vector_.at(i) = static_cast< int >(static_cast< double >(std::rand()) / RAND_MAX * (maxValue_ - minValue_) + minValue_);
    }
    else if (std::is_same< T, double >::value)
    {
      vector_.at(i) = (static_cast< double >(std::rand()) / RAND_MAX * (newMaxValue - newMinValue) + newMinValue) / multiplier;
    }
    else
    {
      throw std::logic_error("Incorrect matrix type!");
    }
  }
}
template< typename T >
pyankov::Matrix< T >::Matrix(size_t rows, size_t columns, const std::vector< T >& vector):
  rows_(rows),
  columns_(columns),
  vector_(vector)
{
  bool isCorrectSize = (rows != 0) && (columns != 0);
  bool isCorrectType = (std::is_same< T, int >::value) || (std::is_same< T, double >::value);
  bool isCorrectVectorSize = vector.size() == (rows * columns);
  if (!isCorrectSize || !isCorrectType || !isCorrectVectorSize)
  {
    throw std::logic_error("Incorrect matrix paramerers!");
  }
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
    throw std::invalid_argument("Can not get element with these indexes!");
  }
  return vector_.at(row * columns_ + column);
}
template< typename T >
size_t pyankov::Matrix< T >::getPrecision() const
{
  return precision_;
}
template< typename T >
void pyankov::Matrix< T >::setElement(size_t row, size_t column, T element)
{
  if (row > rows_ || column > columns_)
  {
    throw std::invalid_argument("Can not set element on these indexes!");
  }
  vector_.at(row * columns_ + column) = element;
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
void pyankov::Matrix< T >::setStaticValues(int minValue, int maxValue, size_t precision)
{
  if (minValue >= maxValue || precision == 0)
  {
    throw std::logic_error("Incorrect matrix class parameters!");
  }
  minValue_ = minValue;
  maxValue_ = maxValue;
  precision_ = precision;
}
template< typename T >
pyankov::Matrix< double > pyankov::Matrix< T >::convertToDouble()
{
  std::vector< double > vector;
  for (size_t i = 0; i < rows_ * columns_; i++)
  {
    vector.push_back(static_cast< double >(vector_.at(i)));
  }
  pyankov::Matrix< double > matrix(rows_, columns_, vector);
  return matrix;
}
template< typename T, typename S >
pyankov::Matrix< T >& pyankov::operator+=(pyankov::Matrix< T >& left, const pyankov::Matrix< S >& right)
{
  bool isCorrectTypes = !(std::is_same< T, int >::value && std::is_same< S, double >::value);
  bool isCorrectSizes = (left.getRows() == right.getRows()) && (left.getColumns() == right.getColumns());
  if (!isCorrectTypes || !isCorrectSizes)
  {
    throw std::logic_error("Can not add these matrices!");
  }
  for (size_t i = 0; i < left.getRows(); i++)
  {
    for (size_t j = 0; j < left.getColumns(); j++)
    {
      T newValue = left.getElement(i, j) + right.getElement(i, j);
      left.setElement(i, j, newValue);
    }
  }
  return left;
}
template< typename T, typename S >
pyankov::Matrix< T >& pyankov::operator-=(pyankov::Matrix< T >& left, const pyankov::Matrix< S >& right)
{
  bool isCorrectTypes = !(std::is_same< T, int >::value && std::is_same< S, double >::value);
  bool isCorrectSizes = (left.getRows() == right.getRows()) && (left.getColumns() == right.getColumns());
  if (!isCorrectTypes || !isCorrectSizes)
  {
    throw std::logic_error("Can not add these matrices!");
  }
  for (size_t i = 0; i < left.getRows(); i++)
  {
    for (size_t j = 0; j < left.getColumns(); j++)
    {
      T newValue = left.getElement(i, j) - right.getElement(i, j);
      left.setElement(i, j, newValue);
    }
  }
  return left;
}
template< typename T, typename S >
pyankov::Matrix< T >& pyankov::operator*=(pyankov::Matrix< T >& left, const pyankov::Matrix< S >& right)
{
  bool isCorrectTypes = !(std::is_same< T, int >::value && std::is_same< S, double >::value);
  bool isCorrectSizes = left.getColumns() == right.getRows();
  if (!isCorrectTypes || !isCorrectSizes)
  {
    throw std::logic_error("Can not multiply these matrices!");
  }
  std::vector< T > vector(left.getRows() * right.getColumns());
  for (size_t i = 0; i < left.getRows(); i++)
  {
    for (size_t j = 0; j < right.getColumns(); j++)
    {
      for (size_t k = 0; k < left.getColumns(); k++)
      {
        vector.at(i * right.getColumns() + j) += left.getElement(i, k) * right.getElement(k, j);
      }
    }
  }
  pyankov::Matrix< T > newMatrix(left.getRows(), right.getColumns(), vector);
  left = newMatrix;
  return left;
}
template< typename T, typename S >
bool pyankov::operator==(const pyankov::Matrix< T >& left, const pyankov::Matrix< S >& right)
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
    matrix = pyankov::Matrix< T >(matrix.getRows(), matrix.getColumns(), vector);
  }
  return in;
}
template< typename T >
std::ostream& pyankov::operator<<(std::ostream& out, const pyankov::Matrix< T >& matrix)
{
  pyankov::ioFmtGuard guard(out);
  out << "size: " << matrix.getRows() << "x" << matrix.getColumns() << "; type: ";
  if (std::is_same< T, int >::value)
  {
    out << "int";
  }
  else
  {
    out << "double";
  }
  out << "; elements:\n";
  T maxElem = std::numeric_limits< T >::min();
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
  size_t addLen = 4;
  size_t maxLen = str.length() + addLen;
  for (size_t i = 0; i < matrix.getRows(); i++)
  {
    out << std::right << std::setw(2) << '|';
    for (size_t j = 0; j < matrix.getColumns(); j++)
    {
      if (std::is_same< T, int >::value)
      {
        out << std::right << std::setw(maxLen) << matrix.getElement(i, j);
      }
      else
      {
        out << std::fixed << std::right << std::setprecision(matrix.getPrecision()) << std::setw(maxLen);
        out << matrix.getElement(i, j);
      }
    }
    out << std::right << " |\n";
  }
  return out;
}
#endif
