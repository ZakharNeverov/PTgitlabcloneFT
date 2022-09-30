#include "graph.hpp"

#include <iostream>
#include <algorithm>

#include <stdexcept>

alhimenko::Graph::Graph(std::list<Vertex_t> vertexes)
{
  std::list<Vertex_t> temp;
  temp = std::move(vertexes);

  try
  {
    auto i = temp.begin();
    while (i != temp.end())
    {
      auto num = i->num_;
      bool isErase = false;
      for(auto&& x : i->edges_)
      {
        if (std::find(temp.begin(), temp.end(), x) == temp.end())
        {
          auto deleteAllEdgesToVertexI = [&num](Vertex_t& y)
          {
             y.edges_.erase(
              std::remove(y.edges_.begin(), y.edges_.end(), num),
              y.edges_.end());
          };

          std::for_each(temp.begin(), temp.end(), deleteAllEdgesToVertexI);

          i = temp.erase(std::remove(temp.begin(), temp.end(), num), temp.end());
          isErase = true;

          break;
        }
      }
      if (!isErase) ++i;
    }
  }
  catch (const std::exception& ex)
  {
    std::cerr << ex.what();
    return;
  }
  for (auto&& i : temp)
  {
    if (i.num_ > visited_max_size) visited_max_size = i.num_ + 1;
  }
  vertexes_ = std::move(temp);
  for (auto&& i : vertexes_)
  {
    visited_.emplace(i.num_, 0);
  }
}

bool alhimenko::Graph::empty() const
{
  return vertexes_.empty();
}

bool alhimenko::Graph::find(const int32_t v_num) const
{
  return std::find(vertexes_.begin(), vertexes_.end(), v_num) != vertexes_.end();
}

bool alhimenko::Graph::isEdge(const int32_t v1, const int32_t v2) const
{
  return std::find_if(vertexes_.begin(), vertexes_.end(),
    [&](Vertex_t x)
    {
      if (x.num_ == v1)
      {
        return std::find(x.edges_.begin(), x.edges_.end(), v2) != x.edges_.end();
      }
      return false;
    }
  ) != vertexes_.end();
}

void alhimenko::Graph::insert(const Vertex_t v)
{
  try
  {
    if (std::find(vertexes_.begin(), vertexes_.end(),v.num_) == vertexes_.end())
    {
      std::for_each(v.edges_.begin(), v.edges_.end(),
        [&](int32_t x)
        {
          if (std::find(vertexes_.begin(), vertexes_.end(), x) == vertexes_.end())
          {
            throw std::runtime_error("Insert error");
          }
        });

      if (v.num_ > visited_max_size) visited_max_size = v.num_ + 1;
      vertexes_.push_back(v);
      visited_.emplace(v.num_, 0);
    }
    else throw std::runtime_error("Insert error");

  }
  catch (std::exception& ex)
  {
    std::cerr << ex.what();
  }
}

void alhimenko::Graph::erase(const int32_t v)
{
  vertexes_.erase(std::remove(vertexes_.begin(), vertexes_.end(), v), vertexes_.end());

  std::for_each(vertexes_.begin(), vertexes_.end(),
    [&](Vertex_t& x)
    {
      x.edges_.erase(std::remove(x.edges_.begin(), x.edges_.end(), v), x.edges_.end());
    }
  );

  for (auto&& it = visited_.begin(); it != visited_.end();)
  {
    if (it->first == v)
    {
      it = visited_.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

void alhimenko::Graph::print(std::ostream& out) const
{
  std::for_each(vertexes_.begin(), vertexes_.end(),
    [&](Vertex_t x)
    {
      std::cout << x.num_ << ": ";
      std::copy(x.edges_.begin(), x.edges_.end(), std::ostream_iterator<uint32_t>(out, " "));
      std::cout << '\n';
    });
}

void alhimenko::Graph::dfs(std::ostream& out) const
{
  for (auto&& i : vertexes_)
  {
    if (visited_.at(i.num_) == 0)
    {
      dfs_supply(i.num_, out);
    }
  }
}

void alhimenko::Graph::dfs_supply(const uint32_t& v_num, std::ostream& out) const
{
  visited_.at(v_num) = 1;

  auto edges = std::find(vertexes_.begin(), vertexes_.end(), v_num)->edges_;
  for (auto&& i : edges)
  {
    if (visited_.at(i) == 0)
    {
      dfs_supply(i, out);
    }
  }

  out << v_num << " ";
}

alhimenko::Vertex_t::Vertex_t(uint32_t num, std::initializer_list<uint32_t> vertexes):
  num_(num), edges_(std::move(vertexes))
{
}

alhimenko::Vertex_t::Vertex_t(uint32_t num, std::list<uint32_t>& vertexes):
  num_(num), edges_(std::move(vertexes))
{
}
