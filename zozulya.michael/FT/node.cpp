#include "node.h"

int zozulya::Node::operator+(const Node& rth) const
{
  return frequency + rth.frequency;
}

int zozulya::Node::get_frequency() const
{
  return frequency;
}

bool zozulya::Node::is_leaf() const
{
  return (left == nullptr && right == nullptr);
}

zozulya::uchar zozulya::Node::get_byte() const
{
  return ch;
}

char zozulya::Node::get_char() const
{
  return static_cast<char>(ch);
}

std::string zozulya::Node::get_name() const
{
  if (ch == 0)
  {
    return name;
  }
  else
  {
    return std::string(1, get_char());
  }
}

std::string zozulya::Node::code() const
{
  return code_string;
}

void zozulya::Node::code(const std::string& c)
{
  code_string = c;
}
