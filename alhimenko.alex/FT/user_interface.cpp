#include "user_interface.hpp"

#include "parser.hpp"

alhimenko::User_interface::User_interface():
  commands_({
  {"CREATE", createGraph},
  {"PRINT", printGraph},
  {"INSERT", insertVertex},
  {"DELETE", deleteVertex},
  {"FIND", findVertex},
  {"CHECK_EDGE", checkEdge},
  {"DFS", dfs}
    })
{
}

void alhimenko::User_interface::start(std::istream& in, std::ostream& out)
{
  try
  {
    Parser< command > parser(commands_);
    parser.start(in, std::cout, in, std::cout);
  }
  catch (const std::exception& ex)
  {
  }
}
