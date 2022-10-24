#include "commands.hpp"
#include <istream>
#include <ostream>

void funtov::addJoit(std::istream& is, Graph& graph)
{
  size_t len = 0;
  std::string first, second;
  is >> first >> second >> len;
  graph.addJoint(first, second, len);
}
void funtov::addApex(std::istream& is, Graph& graph)
{
  std::string name;
  is >> name;
  graph.addApex(name);
}
void funtov::doDfs(Graph& graph)
{
  graph.doDfs();
}
void funtov::deleteGraph(Graph& graph)
{
  graph.deleteGraph();
}
void funtov::topoSort(std::ostream& os, Graph& graph)
{
  std::vector< size_t > const res = graph.topoSort();
  for (unsigned long const re: res)
  {
    os << re << ' ' << std::endl;
  }
}
void funtov::checkGraph(std::ostream& os, Graph& graph)
{
  os << graph << std::endl;
}
void funtov::lookWay(std::istream& is, std::ostream& os, Graph& graph)
{
  std::string first, second;
  is >> first >> second;
  os << graph.getLen(first, second);
}
void funtov::changeWay(std::istream& is, Graph& graph)
{
  std::string first, second;
  size_t len = 0;
  is >> first >> second >> len;
  graph.changeWay(first, second, len);
}
void funtov::deleteWay(std::istream& is, Graph& graph)
{
  std::string first, second;
  is >> first >> second;
  graph.deleteWay(first, second);
}
void funtov::otherWaysFrom(std::istream& is, std::ostream& os, Graph& graph)
{
  std::string apex;
  is >> apex;
  graph.checkOtherWays(os, apex);
}
