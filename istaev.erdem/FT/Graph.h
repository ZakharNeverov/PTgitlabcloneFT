#ifndef GRAPH_H
#define GRAPH_H

#include <iosfwd>
#include <list>
#include <map>

namespace istaev
{
  using adjacencyList = std::map< int, std::list< int > >;
  class Graph
  {
  public:
    bool isEmpty();
    bool isNodeInGraph(int vertex);
    bool isEdgeBetweenNodes(int v1, int v2);
    adjacencyList::iterator insertNode(int v);
    adjacencyList::iterator insertNode(int v, std::list< int >&& nodes);
    void removeVertex(int v);
    void insertEdgeBetweenNodes(int v1, int v2);
    void printGraph(std::ostream& out);
    std::map< int, int > findShortPathsForVertex(int v);
    bool isTree();
    int calculateDiameter();

  private:
    std::map< int, std::list< int > > vertexes;
    std::map< int, int > paths;
    void clearPaths();
    void bfs(int s);
    bool dfs(int s, int parent);
  };
}
#endif
