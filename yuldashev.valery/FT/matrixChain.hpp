#ifndef MATRIX_CHAIN_HPP
#define MATRIX_CHAIN_HPP
#include <string>
#include <map>
#include <functional>
#include <type_traits>
#include "matrix.hpp"
namespace yuldashev
{
  struct MatrixChain
  {
    std::map< std::string, Matrix< int > > iChain;
    std::map< std::string, Matrix< double > > dChain;
  };
  bool existName(const std::string& name, const MatrixChain& matrixChain);
  template < class T >
  using matrix_it = typename std::map< std::string, Matrix< T > >::const_iterator;
  template < class T >
  using matrix_map = std::map< std::string, Matrix< T > >;
  template < class T >
  bool findAndDo(const std::string& name, const matrix_map< T >& matrixChain, std::function< void(const matrix_it< T >) > f)
  {
    if (name.empty())
    {
      throw std::logic_error("Name is empty.");
    }
    auto matrix = matrixChain.find(name);
    if (matrix != matrixChain.end())
    {
      f(matrix);
      return true;
    }
    else
    {
      return false;
    }
  }
  template < class T >
  void getDoubleMatrixFromIt(const matrix_it< T >& it, Matrix< double >& outMatrix)
  {
    Matrix< T > mat = it->second;
    outMatrix.data.clear();
    for (size_t i = 0; i < getRowsCount(mat); i++)
    {
      std::vector< double > column;
      for (size_t j = 0; j < getColumnsCount(mat); j++)
      {
        column.push_back(mat.data[i][j]);
      }
      outMatrix.data.push_back(column);
    }
  }
  template < class T >
  void writeMatrixToVector(const matrix_it< T >& it, std::vector< Matrix< double > >& vectorChain)
  {
    Matrix< double > matrix;
    getDoubleMatrixFromIt< T >(it, matrix);
    vectorChain.push_back(matrix);
  }
  template < class T >
  void doMathOperation(std::istream& in, MatrixChain& chain, std::function< Matrix< T >(const Matrix< T >&, const Matrix< T >&) > f)
  {
    std::string resultname = "";
    in >> resultname;
    if (resultname.empty() || existName(resultname, chain))
    {
      throw std::logic_error("Wrong resultname.");
    }
    std::vector< Matrix< double > > mathChain;
    using namespace std::placeholders;
    std::string name = "";
    do
    {
      in >> name;
      if (name == "")
      {
        continue;
      }
      bool res1 = findAndDo< int >(name, chain.iChain, std::bind(writeMatrixToVector< int >, _1, std::ref(mathChain)));
      bool res2 = findAndDo< double >(name, chain.dChain, std::bind(writeMatrixToVector< double >, _1, std::ref(mathChain)));
      if (!(res1 || res2))
      {
        throw std::logic_error("Name not found.");
      }
    } while (in.peek() != '\n');
    if (mathChain.empty())
    {
      throw std::logic_error("Matrix chain is empty.");
    }
    Matrix< double > matrixResult = mathChain[0];
    for (size_t i = 1; i < mathChain.size(); i++)
    {
      matrixResult = f(matrixResult, mathChain[i]);
    }
    chain.dChain.insert({ resultname, matrixResult });
  }
}
#endif
