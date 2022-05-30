#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <vector>
#include <map>
#include <iosfwd>
#include <string>
#include <iomanip>
#include "iofmtguard.hpp"
namespace yuldashev
{
  template< class T >
  struct Matrix
  {
    std::vector< std::vector< T > > data;
  };
  template< class T >
  size_t getRowsCount(const Matrix< T >& matrix)
  {
    return matrix.data.size();
  }
  template< class T >
  size_t getColumnsCount(const Matrix< T >& matrix)
  {
    return getRowsCount(matrix) > 0 ? matrix.data[0].size() : 0;
  }
}
#endif
