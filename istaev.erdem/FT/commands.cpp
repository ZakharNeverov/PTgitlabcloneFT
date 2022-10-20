#include "commands.h"

#include <iostream>

#include "Messages.h"

namespace
{
  bool correctInput(std::istream& in)
  {
    return !(!in && !in.eof());
  }

  void skip(std::istream& in)
  {
    in.clear();
    in.ignore();
  }
}

istaev::Create::Create(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void istaev::Create::operator()(istaev::storage& graphs)
{
  if (in_.peek() == '\n')
  {
    out_ << "You need to text name of your graph!\n";
    return;
  }
  std::string name = "";
  in_ >> name;
  if (!correctInput(in_))
  {
    invalidArguments(out_) << "\n";
    skip(in_);
    return;
  }
  auto it = graphs.find(name);
  if (it != graphs.end())
  {
    out_ << "Graph is already present!\n";
    return;
  }
  else
  {
    istaev::Graph graph;
    graphs.emplace(name, graph);
  }
}

istaev::PrintGraph::PrintGraph(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void istaev::PrintGraph::operator()(istaev::storage& graphs)
{
  if (in_.peek() == '\n')
  {
    out_ << "You need to text name of your graph!\n";
    return;
  }
  std::string name = "";
  in_ >> name;
  if (!correctInput(in_))
  {
    invalidArguments(out_) << "\n";
    skip(in_);
    return;
  }
  auto graph = graphs.find(name);
  if (graph == graphs.end())
  {
    out_ << "Graph with name " << name << " is absent!\n";
    return;
  }
  if (graph->second.isEmpty())
  {
    istaev::graphIsEmpty(out_) << "\n";
    return;
  }
  graph->second.printGraph(out_);
}

istaev::AddVertex::AddVertex(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void istaev::AddVertex::operator()(istaev::storage& graphs)
{
  if (in_.peek() == '\n')
  {
    out_ << "You need to text name of your graph!\n";
    return;
  }
  std::string name = "";
  int v;
  in_ >> name;
  auto graph = graphs.find(name);
  if (graph == graphs.end())
  {
    out_ << "Graph with name " << name << " is absent!\n";
    return;
  }

  if (in_.peek() == '\n')
  {
    out_ << "Not enough args!\n";
    return;
  }

  in_ >> v;
  if (!correctInput(in_))
  {
    invalidArguments(out_) << "\n";
    skip(in_);
    return;
  }

  if (in_.peek() == '\n')
  {
    graph->second.insertNode(v);
    return;
  }
  std::list< int > nbrs;
  std::string tmp;
  in_ >> tmp;
  while (tmp.length() != 0)
  {
    int node;
    try
    {
      size_t inx = tmp.find(',');
      node = (inx == std::string::npos) ? std::stoi(tmp) : std::stoi(tmp.substr(0, inx));
      nbrs.push_back(node);
      if (inx == std::string::npos)
      {
        tmp.clear();
      }
      else
      {
        tmp.erase(0, inx + 1);
      }
    }
    catch (const std::exception& ex)
    {
      invalidArguments(out_) << "\n";
      return;
    }
  }
  graph->second.insertNode(v, std::move(nbrs));
}

istaev::RemoveVertex::RemoveVertex(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void istaev::RemoveVertex::operator()(istaev::storage& graphs)
{
  if (in_.peek() == '\n')
  {
    out_ << "You need to text name of your graph!\n";
    return;
  }
  std::string name = "";
  int v;
  in_ >> name;
  auto graph = graphs.find(name);
  if (graph == graphs.end())
  {
    out_ << "Graph with name " << name << " is absent!\n";
    return;
  }

  if (in_.peek() == '\n')
  {
    out_ << "Not enough args!\n";
    return;
  }

  in_ >> v;

  if (!correctInput(in_))
  {
    invalidArguments(out_) << "\n";
    skip(in_);
    return;
  }

  if (!graph->second.isEmpty() && graph->second.isNodeInGraph(v))
  {
    graph->second.removeVertex(v);
  }
}

istaev::InsertEdge::InsertEdge(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void istaev::InsertEdge::operator()(istaev::storage& graphs)
{
  if (in_.peek() == '\n')
  {
    out_ << "You need to text name of your graph!\n";
    return;
  }
  std::string name = "";
  int v1, v2;
  in_ >> name;
  auto graph = graphs.find(name);
  if (graph == graphs.end())
  {
    out_ << "Graph with name " << name << " is absent!\n";
    return;
  }

  if (in_.peek() == '\n')
  {
    out_ << "Not enough args!\n";
    return;
  }

  in_ >> v1;
  if (!correctInput(in_))
  {
    invalidArguments(out_) << "\n";
    skip(in_);
    return;
  }

  if (in_.peek() == '\n')
  {
    invalidArguments(out_) << "\n";
    return;
  }

  in_ >> v2;
  if (!correctInput(in_))
  {
    invalidArguments(out_) << "\n";
    skip(in_);
    return;
  }
  graph->second.insertEdgeBetweenNodes(v1, v2);
}

istaev::CheckEdge::CheckEdge(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void istaev::CheckEdge::operator()(istaev::storage& graphs)
{
  if (in_.peek() == '\n')
  {
    out_ << "You need to text name of your graph!\n";
    return;
  }
  std::string name = "";
  int v1, v2;
  in_ >> name;
  auto graph = graphs.find(name);
  if (graph == graphs.end())
  {
    out_ << "Graph with name " << name << " is absent!\n";
    return;
  }

  if (in_.peek() == '\n')
  {
    out_ << "Not enough args!\n";
    return;
  }

  in_ >> v1;
  if (!correctInput(in_))
  {
    invalidArguments(out_) << "\n";
    skip(in_);
    return;
  }

  if (in_.peek() == '\n')
  {
    invalidArguments(out_) << "\n";
    return;
  }

  in_ >> v2;
  if (!correctInput(in_))
  {
    invalidArguments(out_) << "\n";
    skip(in_);
    return;
  }

  if (!graph->second.isEmpty())
  {
    if (graph->second.isEdgeBetweenNodes(v1, v2))
    {
      istaev::edgeIsPresent(out_) << '\n';
    }
    else
    {
      istaev::edgeIsAbsent(out_) << '\n';
    }
  }
}

istaev::Bfs::Bfs(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void istaev::Bfs::operator()(istaev::storage& graphs)
{
  if (in_.peek() == '\n')
  {
    out_ << "You need to text name of your graph!\n";
    return;
  }
  std::string name = "";
  int v;
  in_ >> name;
  if (in_.peek() == '\n')
  {
    out_ << "Not enough args!\n";
    return;
  }
  in_ >> v;
  if (!correctInput(in_))
  {
    invalidArguments(out_) << "\n";
    skip(in_);
    return;
  }
  auto graph = graphs.find(name);
  if (graph == graphs.end())
  {
    out_ << "Graph with name " << name << " is absent!\n";
    return;
  }
  if (graph->second.isEmpty() || !graph->second.isNodeInGraph(v))
  {
    istaev::vertexIsAbsent(out_) << '\n';
    return;
  }
  auto paths = graph->second.findShortPathsForVertex(v);
  for (auto& it : paths)
  {
    out_ << it.first << " - " << it.second << '\n';
  }
}

istaev::CalcDiameter::CalcDiameter(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void istaev::CalcDiameter::operator()(istaev::storage& graphs)
{
  std::string name = "";
  in_ >> name;
  auto graph = graphs.find(name);
  if (graph == graphs.end())
  {
    out_ << "Graph with name " << name << " is absent!\n";
    return;
  }
  if (graph->second.isTree())
  {
    out_ << "Diameter of tree = " << graph->second.calculateDiameter() << '\n';
  }
  else
  {
    out_ << "Graph is not a tree!" << '\n';
  }
}
