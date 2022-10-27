#ifndef COMMANDS_H
#define COMMANDS_H

#include <iosfwd>
#include <string>

#include "Graph.h"

namespace istaev
{
  using storage = std::map< std::string, istaev::Graph >;

  struct Create
  {
    Create() = delete;
    Create(std::istream& in, std::ostream& out);
    void operator()(storage& graph);

  private:
    std::istream& in_;
    std::ostream& out_;
  };

  struct PrintGraph
  {
    PrintGraph() = delete;
    PrintGraph(std::istream& in, std::ostream& out);
    void operator()(storage& graph);

  private:
    std::istream& in_;
    std::ostream& out_;
  };

  struct AddVertex
  {
    AddVertex() = delete;
    AddVertex(std::istream& in, std::ostream& out);
    void operator()(storage& graph);

  private:
    std::istream& in_;
    std::ostream& out_;
  };

  struct RemoveVertex
  {
    RemoveVertex() = delete;
    RemoveVertex(std::istream& in, std::ostream& out);
    void operator()(storage& graph);

  private:
    std::istream& in_;
    std::ostream& out_;
  };

  struct InsertEdge
  {
    InsertEdge() = delete;
    InsertEdge(std::istream& in, std::ostream& out);
    void operator()(storage& graph);

  private:
    std::istream& in_;
    std::ostream& out_;
  };

  struct CheckEdge
  {
    CheckEdge() = delete;
    CheckEdge(std::istream& in, std::ostream& out);
    void operator()(storage& graph);

  private:
    std::istream& in_;
    std::ostream& out_;
  };

  struct Bfs
  {
    Bfs() = delete;
    Bfs(std::istream& in, std::ostream& out);
    void operator()(storage& graph);

  private:
    std::istream& in_;
    std::ostream& out_;
  };

  struct CalcDiameter
  {
    CalcDiameter() = delete;
    CalcDiameter(std::istream& in, std::ostream& out);
    void operator()(storage& graph);

  private:
    std::istream& in_;
    std::ostream& out_;
  };
}
#endif

