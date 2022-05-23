#ifndef COMMANDS_H
#define COMMANDS_H
#include <string>
#include<iostream>
#include <list>
#include "graph.hpp"

namespace mitkov {
  class Command {
  private:
    std::map< std::string, mitkov::orientedGraph< int > >& allGraphs;
    std::ostream& stream;
    std::map< std::string, void (mitkov::Command::*)(const std::list< std::string >& args) > commands;

    void search(const std::list< std::string >& args);
    void getMaxDegree(const std::list< std::string >& args);
    void topologicSort(const std::list< std::string >& args);
    void getCycle(const std::list< std::string >& args);
    void merge(const std::list< std::string >& args);
    void add(const std::list< std::string >& args);
    void rAdd(const std::list< std::string >& args);
    void addFirst(const std::list< std::string >& args);
  public:
    Command(std::map< std::string, mitkov::orientedGraph< int > >& allGraphs, std::ostream& stream);
    
    void operator()(std::list< std::string >& cmdList);
  };
}
#endif

