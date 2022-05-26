#include "CommandSet.hpp"

#include <iostream>
#include <limits>

semichev::CommandSet::CommandSet(std::initializer_list< commands_t::value_type > il, std::istream& in):
  commands_(il),
  in_(in)
{}

void semichev::CommandSet::operator()()
{
  std::string arg;
  in_ >> arg;

  auto cur = commands_.find(arg);
  if (cur == commands_.end())
  {
    in_.setstate(in_.failbit);
  }

  if (in_)
  {
    cur->second->operator()();
  }
}
