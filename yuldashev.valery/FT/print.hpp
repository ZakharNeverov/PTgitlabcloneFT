#ifndef PRINT_HPP
#define PRINT_HPP
#include <iomanip>
#include <iostream>
#include <map>
#include <iterator>
#include "iofmtguard.hpp"
#include "matrix.hpp"
namespace yuldashev
{
  std::ostream& printInvalid(std::ostream& out);
  template < class T >
  using matrix_it = typename std::map< std::string, Matrix< T > >::const_iterator;
  template< class T >
  void print(std::ostream& out, const matrix_it< T >& it, bool isFile)
  {
    Matrix< T > matrix = it->second;
    size_t rows = getRowsCount(matrix);
    size_t columns = getColumnsCount(matrix);
    iofmtguard io(out);
    if (isFile)
    {
      out << rows << " " << columns << "\n";
    }
    out << std::fixed << std::setprecision(1);
    for (size_t i = 0; i < rows; i++)
    {
      for (size_t j = 0; j < columns; j++)
      {
        out << matrix.data[i][j];
        out << (j + 1 == columns ? "" : " ");
      }
      out << "\n";
    }
  }
}
#endif
