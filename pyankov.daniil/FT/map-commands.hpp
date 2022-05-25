#ifndef MAP_COMMANDS_HPP
#define MAP_COMMANDS_HPP
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
  void create(chain_t& chain, std::istream& in);
  void add(chain_t& chain, std::istream& in);
  void sub(chain_t& chain, std::istream& in);
  void multiply(chain_t& chain, std::istream& in);
  void concat(chain_t& chain, std::istream& in);
  void print(const chain_t& chain, std::istream& in, std::ostream& out);
  void equals(const chain_t& chain, std::istream& in, std::ostream& out);
}
#endif
