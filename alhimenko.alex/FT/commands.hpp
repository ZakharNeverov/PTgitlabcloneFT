#include <iosfwd>
#include <map>
#include <memory>
#include <string>

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "graph.hpp"

namespace alhimenko
{
  namespace detail
  {
    static std::map<std::string, std::unique_ptr<Graph>> graphs;

    void createGraph(std::istream&, std::ostream&);
    void printGraph(std::istream&, std::ostream&);
    void insertVertex(std::istream&, std::ostream&);
    void deleteVertex(std::istream&, std::ostream&);
    void findVertex(std::istream&, std::ostream&);
    void checkEdge(std::istream&, std::ostream&);
    void dfs(std::istream&, std::ostream&);

    bool correctInput(std::istream&);

    Vertex_t readVertex(std::istream&, bool& );
    bool checkName(const std::string&);
  }
}
#endif
