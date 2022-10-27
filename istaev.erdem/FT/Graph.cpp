#include "Graph.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <queue>

bool istaev::Graph::isEmpty()
{
  return vertexes_.empty();
}

bool istaev::Graph::isNodeInGraph(int vertex)
{
  return vertexes_.find(vertex) != vertexes_.end();
}

bool istaev::Graph::isEdgeBetweenNodes(int v1, int v2)
{
  auto it = vertexes_.find(v1);
  return !(it == vertexes_.end()) && std::find(it->second.begin(), it->second.end(), v2) != it->second.end();
}

std::map< int, std::list< int > >::iterator istaev::Graph::insertNode(int v, std::list< int >&& nodes)
{
  for (auto it: nodes)
  {
    insertEdgeBetweenNodes(v, it);
  }
  auto result = vertexes_.insert(std::make_pair(v, std::move(nodes)));
  paths_.emplace(v, -1);
  return result.first;
}

istaev::adjacencyList::iterator istaev::Graph::insertNode(int v)
{
  return insertNode(v, {});
}

void istaev::Graph::removeVertex(int v)
{
  vertexes_.erase(v);
  for (auto it = vertexes_.begin(); it != vertexes_.end(); ++it)
  {
    auto elem = std::find(it->second.begin(), it->second.end(), v);
    if (elem != it->second.end())
    {
      it->second.erase(elem);
    }
  }
}

void istaev::Graph::insertEdgeBetweenNodes(int v1, int v2)
{
  if (!isEdgeBetweenNodes(v1, v2))
  {
    auto first = insertNode(v1);
    auto second = insertNode(v2);
    first->second.push_back(v2);
    second->second.push_back(v1);
  }
}

void istaev::Graph::printGraph(std::ostream& out)
{
  for (auto it: vertexes_)
  {
    out << it.first << " -> ";
    std::copy(it.second.begin(), it.second.end(), std::ostream_iterator< int >(out, " "));
    out << "\n";
  }
}

void istaev::Graph::bfs(int s)
{
  clearPaths();
  paths_[s] = 0;
  std::queue< int > q;
  q.push(s);
  while (!q.empty())
  {
    int v = q.front();
    q.pop();
    for (int to: vertexes_[v])
    {
      if (paths_[to] == -1)
      {
        paths_[to] = paths_[v] + 1;
        q.push(to);
      }
    }
  }
}

void istaev::Graph::clearPaths()
{
  if (isEmpty())
  {
    return;
  }
  auto it = paths_.begin();
  while (it != paths_.end())
  {
    it->second = -1;
    ++it;
  }
}

std::map< int, int > istaev::Graph::findShortPathsForVertex(int v)
{
  bfs(v);
  return paths_;
}

bool istaev::Graph::dfs(int s, int parent)
{
  paths_[s] = 1;
  for (int v: vertexes_[s])
  {
    if (paths_[v] == -1)
    {
      if (!dfs(v, s))
      {
        return false;
      }
    }
    else if (v != parent)
    {
      return false;
    }
  }
  return true;
}

bool istaev::Graph::isTree()
{
  clearPaths();
  if (!isEmpty() && dfs(vertexes_.begin()->first, -1))
  {
    for (auto& it: paths_)
    {
      if (it.second == -1)
      {
        return false;
      }
    }
    return true;
  }
  return false;
}

int istaev::Graph::calculateDiameter()
{
  if (isEmpty())
  {
    return 0;
  }
  int v = vertexes_.begin()->first;
  bfs(v);
  int maxPath = 0;
  for (auto& el: paths_)
  {
    if (el.second > maxPath)
    {
      maxPath = el.second;
      v = el.first;
    }
  }
  bfs(v);
  maxPath = 0;
  for (auto& el: paths_)
  {
    if (el.second > maxPath)
    {
      maxPath = el.second;
      v = el.first;
    }
  }
  return maxPath;
}

