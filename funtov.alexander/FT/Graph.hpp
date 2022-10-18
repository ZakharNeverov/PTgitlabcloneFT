#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <map>
#include <string>
#include <vector>

namespace funtov
{
  class Graph
  {
  public:
    Graph();

    void checkGraph();
    void deleteGraph();
    size_t getApexCnt() const;
    std::vector< size_t > doDfs();
    std::vector< size_t > topoSort();
    void addApex(const std::string& apexName);
    void checkOtherWays(std::ostream& os, const std::string& apexName);
    size_t getLen(const std::string& first, const std::string& second);
    void dfs(size_t v, std::vector< bool >& used, std::vector< size_t >& ans);
    void deleteWay(const std::string& apexName1, const std::string& apexName2);
    void addJoint(const std::string& apexName1, const std::string& apexName2, size_t length);
    void changeWay(const std::string& apexName1, const std::string& apexName2, size_t length);

    friend std::istream& operator>>(std::istream& is, Graph& other);
    friend std::ostream& operator<<(std::ostream& os, Graph& other);

  private:
    std::vector< std::vector< size_t > > plan;
    size_t count;
    std::map< std::string, size_t > pm;
    std::map< size_t, std::string > mp;
  };
  std::istream& operator>>(std::istream& is, Graph& other);
  std::ostream& operator<<(std::ostream& os, Graph& other);
}
#endif
