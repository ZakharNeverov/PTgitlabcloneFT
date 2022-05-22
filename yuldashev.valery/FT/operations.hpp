#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP
#include <iostream>
#include <string>
#include <limits>
#include "matrix.hpp"
namespace yuldashev
{
  template < class T >
  using matrix_it = typename std::map< std::string, Matrix< T > >::const_iterator;
  template < class T >
  using matrix_map = std::map< std::string, Matrix< T > >;
  template < class T >
  void deleteMatrix(const matrix_it< T >& it, matrix_map< T >& chain)
  {
    chain.erase(it);
  }
  template < class T >
  void replace(const matrix_it< T >& it, matrix_map< T >& chain, std::string& name, const size_t row, const size_t col, const T num)
  {
    Matrix< T > matrix = it->second;
    if (row > getRowsCount(it->second) || col > getColumnsCount(it->second))
    {
      throw std::logic_error("Wrong row/column.");
    }
    matrix.data[row][col] = num;
    deleteMatrix(it, chain);
    chain.insert({ name, matrix });
  }
  template< class T >
  Matrix< T > sum(const Matrix<T>& left, const Matrix< T >& right)
  {
    if (getRowsCount(left) != getRowsCount(right) || getColumnsCount(left) != getColumnsCount(right))
    {
      throw std::logic_error("Wrong arguments.");
    }
    Matrix< T > result;
    for (size_t i = 0; i < getRowsCount(left); i++)
    {
      std::vector< T > column;
      for (size_t j = 0; j < getColumnsCount(left); j++)
      {
        column.push_back(left.data[i][j] + right.data[i][j]);
      }
      result.data.push_back(column);
    }
    return result;
  }
  template < class T >
  Matrix < T > multiply(const Matrix< T >& left, const Matrix< T >& right)
  {
    if (getColumnsCount(left) != getRowsCount(right))
    {
      throw std::logic_error("Wrong arguments.");
    }
    Matrix< T > result;
    for (size_t i = 0; i < getRowsCount(left); i++)
    {
      std::vector< T > column;
      for (size_t j = 0; j < getColumnsCount(left); j++)
      {
        T num = 0;
        for (size_t m = 0; m < getColumnsCount(left); m++)
        {
          num += left.data[i][m] * right.data[m][j];
        }
        column.push_back(num);
      }
      result.data.push_back(column);
    }
    return result;
  }
  template < class T >
  Matrix < T > multiplyByElement(const Matrix< T >& left, const Matrix< T >& right)
  {
    if (getRowsCount(left) != getRowsCount(right) || getColumnsCount(left) != getColumnsCount(right))
    {
      throw std::logic_error("Wrong arguments.");
    }
    Matrix< T > result;
    for (size_t i = 0; i < getRowsCount(left); i++)
    {
      std::vector< T > column;
      for (size_t j = 0; j < getColumnsCount(left); j++)
      {
        column.push_back(left.data[i][j] * right.data[i][j]);
      }
      result.data.push_back(column);
    }
    return result;
  }
  template < class T >
  bool setDataToMatrix(std::istream& in, Matrix< T >& matrix)
  {
    size_t numRows = 0;
    size_t numColumns = 0;
    in >> numRows;
    in >> numColumns;
    if (in.rdstate() == std::ios::failbit)
    {
      return false;
    }
    for (size_t i = 0; i < numRows; i++)
    {
      std::vector< T > columns;
      for (size_t j = 0; j < numColumns; j++)
      {
        if (in.eof())
        {
          throw std::logic_error("Wrong matrix input.");
        }
        T num = 0;
        in >> num;
        columns.push_back(num);
        if (in.rdstate() == std::ios::failbit)
        {
          return false;
        }
      }
      matrix.data.push_back(columns);
    }
    return true;
  }
}
#endif
