#include "user_interface.hpp"

#include "parser.hpp"

alhimenko::User_interface::User_interface():
  commands_({
  {"CREATE", detail::createGraph},
  {"PRINT", detail::printGraph},
  {"INSERT", detail::insertVertex},
  {"DELETE", detail::deleteVertex},
  {"FIND", detail::findVertex},
  {"CHECK_EDGE", detail::checkEdge},
  {"DFS", detail::dfs}
    })
{
}

void alhimenko::User_interface::start(std::istream& in, std::ostream& out)
{
  try
  {
    Parser<command> parser(commands_);
    parser.start(in, std::cout, in, std::cout);
  }
  catch (...)
  {
  }
}
