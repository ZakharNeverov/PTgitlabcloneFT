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
    using pointer = std::shared_ptr<Node>;
    pointer parent{ nullptr };
    pointer left{ nullptr };
    pointer right{ nullptr };
    Node() = default;
    Node(uchar uch, int f): ch(uch), frequency(f) {}
    Node(const std::string& n, int f): name(n), frequency(f) {}

    int operator + (const Node& rth) const;
    friend std::ostream& operator<<(std::ostream& os, const Node& t);
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
    int   frequency{ 0 };
    std::string code_string{ "" };
  };

  using pointer = Node::pointer;

  inline std::ostream& operator<<(std::ostream& os, const Node& node)
  {
    return os << "[" << node.get_name() << "] = " << node.frequency;
  }

  class Greater
  {
    public:
      bool operator()(const pointer& lth, const pointer& rth) const
      {
        return lth->get_frequency() > rth->get_frequency();
      }
  };
}
#endif
