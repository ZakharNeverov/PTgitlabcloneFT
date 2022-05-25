#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <map>
#include <utility>
#include <vector>
#include <string>
#include "matrix.hpp"
namespace pyankov
{
  using int_chain_t = std::map< std::string, Matrix< int > >;
  using double_chain_t = std::map< std::string, Matrix< double > >;
  using chain_t = std::pair< int_chain_t, double_chain_t >;
  bool isContainingName(const chain_t& chain, const std::string& name);
  bool compareMatricesByName(const chain_t& chain, const std::string& left, const std::string& right);
  void doCreateIntMatrix(chain_t& chain, std::istream& in);
  void doCreateDoubleMatrix(chain_t& chain, std::istream& in);
  void concatToRight(chain_t& chain, const std::string& left, const std::string& right, const std::string& result);
  void concatToBot(chain_t& chain, const std::string& left, const std::string& right, const std::string& result);
  void concatToRightInt(chain_t& chain, const std::string& left, const std::string& right, const std::string& result);
  void concatToBotInt(chain_t& chain, const std::string& left, const std::string& right, const std::string& result);
  void concatToRightDouble(chain_t& chain, const std::string& left, const std::string& right, const std::string& result);
  void concatToBotDouble(chain_t& chain, const std::string& left, const std::string& right, const std::string& result);
}
#endif
