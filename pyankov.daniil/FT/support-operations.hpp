#ifndef SUPPORT_OPERATIONS_HPP
#define SUPPORT_OPERATIONS_HPP
#include <map>
#include <string>
#include "matrix.hpp"
namespace pyankov
{
  using matrices_t = std::map< std::string, pyankov::Matrix< double > >;
  void concatToRight(matrices_t& matrices, const Matrix< double >& left, const Matrix< double >& right, const std::string& name);
  void concatToBot(matrices_t& matrices, const Matrix< double >& left, const Matrix< double >& right, const std::string& name);
}
#endif
