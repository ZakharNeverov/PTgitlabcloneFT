#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <list>
#include <cstdint>
#include <initializer_list>
#include <iosfwd>
#include <map>

namespace alhimenko
{
  struct Vertex_t
  {
    uint32_t num_;
    std::list<uint32_t> edges_;

    Vertex_t() = default;
    explicit Vertex_t(uint32_t num, std::initializer_list<uint32_t> vertexes);
    explicit Vertex_t(uint32_t num, std::list<uint32_t>& vertexes);

    inline bool operator==(const Vertex_t& rhs) const
    {
      return this->num_ == rhs.num_;
    }

    inline bool operator==(const int32_t& rhs) const
    {
      return this->num_ == rhs;
    }

    ~Vertex_t() = default;
  };

  class Graph
  {
  public:
    Graph() = default;
    explicit Graph(std::list<Vertex_t>);
    ~Graph() = default;

    bool empty() const;
    bool find(const int32_t) const;
    bool isEdge(const int32_t, const int32_t) const;
    void insert(Vertex_t);
    void erase(int32_t);
    void print(std::ostream&) const;
    void dfs(std::ostream&) const;

  private:
    std::list<Vertex_t> vertexes_;
    mutable std::map<uint32_t, int8_t> visited_;

    uint32_t visited_max_size = 0;

    void dfs_supply(const uint32_t&, std::ostream&) const;
  };
}

#endif
