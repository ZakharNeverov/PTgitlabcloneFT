#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <map>
#include <string>
#include "matrix.hpp"
namespace pyankov
{
  using matrices_t = std::map< std::string, pyankov::Matrix< double > >;
  void create(matrices_t& matrices, std::istream& in);
  void multiply(matrices_t& matrices, std::istream& in);
  void add(matrices_t& matrices, std::istream& in);
  void sub(matrices_t& matrices, std::istream& in);
  void concat(matrices_t& matrices, std::istream& in);
  void print(const matrices_t& matrices, std::istream& in, std::ostream& out);
  void equals(const matrices_t& matrices, std::istream& in, std::ostream& out);
}
#endif
