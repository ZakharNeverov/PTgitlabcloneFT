#ifndef GRAPH_LIST_HPP
#define GRAPH_LIST_HPP

#include <map>
#include <memory>
#include <string>

#include "graph.hpp"

namespace alhimenko
{
  std::map< std::string, std::unique_ptr< Graph > > graphs;
}
#endif
