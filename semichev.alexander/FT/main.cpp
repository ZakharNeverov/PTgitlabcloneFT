#include <iostream>
#include <fstream>
#include <limits>

#include "Expression.hpp"
#include "FunctionIO.hpp"
#include "Cell.hpp"
#include "Spreadsheet.hpp"
#include "CommandSet.hpp"
#include "Functions.hpp"

namespace smcv = semichev;

int main(int argc, char* argv[])
{
  std::ifstream in;
  if (argc == 3)
  {
    in.open(argv[1]);
    if (!in.is_open())
    {
      std::cerr << "unable to open file\n";
      return 1;
    }
  }
  else
  {
    std::cerr << "too many arguments\n";
    return 1;
  }

  smcv::Spreadsheet ws;
  smcv::Enter enter(ws, in);
  while (!in.eof())
  {
    if (!in)
    {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    enter();
  }
  in.close();

  smcv::CommandSet commands(
    {
      { "sheets", std::make_shared< smcv::Sheets >(ws, std::cout) },
      { "delete", std::make_shared< smcv::DeleteSheet >(ws, std::cin) },
      { "enter", std::make_shared< smcv::Enter >(ws, std::cin) },
      { "print", std::make_shared< smcv::Print >(ws, std::cin, std::cout) },
      { "peek", std::make_shared< smcv::Peek >(ws, std::cin, std::cout) },
      { "copy", std::make_shared< smcv::Copy >(ws, std::cin) },
      { "paste", std::make_shared< smcv::Paste >(ws, std::cin) },
    },
    std::cin);

  while (!std::cin.eof())
  {
    if (!std::cin)
    {
      std::cerr << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    commands();
  }

  std::ofstream out(argv[2]);
  if (!out.is_open())
  {
    std::cerr << "unable to open file\n";
    return 1;
  }

  smcv::Peek peek(ws, std::cin, out);
  for (auto&& sheet: ws.getSheets())
  {
    out << "sheet " << sheet.first << '\n';
    peek(sheet.first);
  }
}
