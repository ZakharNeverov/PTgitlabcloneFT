#ifndef IO_GAPH_HPP
#define IO_GAPH_HPP

#include <iosfwd>

#include "graph.hpp"

namespace sviridov
{
  std::istream& operator>>(std::istream& is, MatrixGraph& matrixGraph);
  std::ostream& operator<<(std::ostream& out, MatrixGraph& matrixGraph);
}

#endif
