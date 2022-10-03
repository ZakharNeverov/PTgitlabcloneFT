#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "graph.hpp"

#include <string>
#include <map>
#include <list>
#include <functional>
#include <set>

namespace sviridov
{
  using CommandArgs = std::list< std::string >;
  using MapOfGraphs = std::map < std::string, sviridov::MatrixGraph >;
  using Commands = std::map< std::string, std::function< void(CommandArgs&) > >;
  class Command
  {
  public:
    Command(MapOfGraphs& data, std::ostream& out);
    void operator()(CommandArgs& args);
  private:
    MapOfGraphs& mapOfGraphs_;
    std::ostream& out_;
    Commands mapOfCommands_;

    void getGraphs(CommandArgs& args);
    void print(CommandArgs& args);
    void inVertexesDegrees(CommandArgs& args);
    void outVertexesDegrees(CommandArgs& args);
    void vertexWithMaxDegree(CommandArgs& args);
    void removeMultipleArcs(CommandArgs& args);
    void removeCycles(CommandArgs& args);
    void removeHamiltonianCycles(CommandArgs& args);
    void sourceVertexes(CommandArgs& args);
    void sinkVertexes(CommandArgs& args);
    void adjacentVertexes(CommandArgs& args);
    void path(CommandArgs& args);
    void saveGraph(CommandArgs& args);
  };

  bool getNumber(std::string str, size_t res);
}

#endif
