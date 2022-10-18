#ifndef commands_hpp
#define commands_hpp

#include <string>
#include <vector>
#include "Graph.hpp"

namespace funtov
{
  void addJoit(std::istream& is, Graph& graph);
  void addApex(std::istream& is, Graph& graph);
  void doDfs(Graph& graph);
  void deleteGraph(Graph& graph);
  void topoSort(std::ostream& os, Graph& graph);
  void checkGraph(std::ostream& os, Graph& graph);
  void lookWay(std::istream& is, std::ostream& os, Graph& graph);
  void changeWay(std::istream& is, Graph& graph);
  void deleteWay(std::istream& is, Graph& graph);
  void otherWaysFrom(std::istream& is, std::ostream& os, Graph& graph);
}
#endif
