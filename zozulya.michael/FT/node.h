#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <memory>

namespace zozulya
{
  using uchar = unsigned char;

  class Node
  {
  public:
    using pointer = std::shared_ptr< Node >;
    pointer parent{ nullptr };
    pointer left{ nullptr };
    pointer right{ nullptr };
    Node() = default;
    Node(uchar uch, int f);
    Node(const std::string& n, int f);
    int operator+(const Node& rth) const;
    int get_frequency() const;
    bool is_leaf() const;
    uchar get_byte() const;
    char get_char() const;
    std::string get_name() const;
    std::string code() const;
    void code(const std::string& c);
  private:
    std::string name{ "" };
    uchar ch{ 0 };
    int frequency{ 0 };
    std::string code_string{ "" };
  };

  using pointer = Node::pointer;
  struct Greater
  {
    bool operator()(const pointer& lth, const pointer& rth) const
    {
      return lth->get_frequency() > rth->get_frequency();
    }
  };
}
#endif
