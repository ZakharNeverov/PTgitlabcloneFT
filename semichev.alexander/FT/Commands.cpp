#include "Commands.hpp"

#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iomanip>
#include <iterator>

#include "Formatter.hpp"
#include "FunctionIO.hpp"
#include "Spreadsheet.hpp"

namespace
{
  struct Generator
  {
    int val;
    int operator()()
    {
      return val++;
    }
  };
}

semichev::Sheets::Sheets(Spreadsheet& ws, std::ostream& out):
  ws_(ws),
  out_(out)
{}

void semichev::Sheets::operator()()
{
  for (auto&& sheet: ws_.getSheets())
  {
    out_ << sheet.first << ' ';
  }
  out_ << '\n';
}

semichev::DeleteSheet::DeleteSheet(Spreadsheet& ws, std::istream& in):
  ws_(ws),
  in_(in)
{}

void semichev::DeleteSheet::operator()()
{
  std::string str;
  if (!(in_ >> str))
  {
    return;
  }

  ws_.deleteSheet(str);
}

semichev::Enter::Enter(Spreadsheet& ws, std::istream& in):
  ws_(ws),
  in_(in)
{}

void semichev::Enter::operator()()
{
  std::string str;
  if (!(in_ >> str))
  {
    return;
  }

  try
  {
    if (str == "sheet")
    {
      in_ >> str;
      ws_.setCurrent(str);
    }
    else
    {
      if (ws_.isEmpty())
      {
        in_.setstate(in_.failbit);
        return;
      }
      std::pair< int, int > cell = toExprElem(str).getAddress().getCell();
      FunctionIO funcIO;
      if (in_ >> funcIO)
      {
        ws_.getCurrent()[cell.first][cell.second] = Cell(funcIO.func);
      }
    }
  }
  catch (...)
  {
    in_.setstate(in_.failbit);
  }
}

semichev::Print::Print(Spreadsheet& ws, std::istream& in, std::ostream& out):
  ws_(ws),
  in_(in),
  out_(out)
{}

void semichev::Print::operator()()
{
  if (ws_.isEmpty())
  {
    in_.setstate(in_.failbit);
    return;
  }

  std::string str;
  if (!(in_ >> str))
  {
    return;
  }

  if (str == "range")
  {
    std::string cells[2];
    in_ >> cells[0] >> cells[1];
    std::pair< int, int > from;
    std::pair< int, int > to;
    try
    {
      from = toExprElem(cells[0]).getAddress().getCell();
      to = toExprElem(cells[1]).getAddress().getCell();
    }
    catch (...)
    {
      in_.setstate(in_.failbit);
      return;
    }
    auto rows = std::minmax(from.first, to.first);
    auto cols = std::minmax(from.second, to.second);
    out_ << "\t(";
    std::generate_n(std::ostream_iterator< int >(out_, ")\t("), cols.second - cols.first, Generator{cols.first});
    out_ << cols.second << ")\n\n";
    for (int i = rows.first; i <= rows.second; i++)
    {
      out_ << i << ')';
      auto it = ws_.getCurrent().find(i);
      if (it != ws_.getCurrent().end())
      {
        out_ << "\t";
        int t = cols.first;
        for (auto j = it->second.lower_bound(cols.first); j != it->second.upper_bound(cols.second); j++)
        {
          std::fill_n(std::ostream_iterator< std::string >(out_), j->first - t, "\t");
          t = j->first;
          try
          {
            Formatter f(out_);
            out_ << std::setprecision(5) << j->second.getValue(ws_);
          }
          catch (...)
          {
            out_ << "<INV!>";
          }
        }
      }
      out_ << '\n';
    }
  }
  else
  {
    try
    {
      auto cell = toExprElem(str).getAddress().getCell();
      double result = ws_.getCurrent().at(cell.first).at(cell.second).getValue(ws_);
      std::cout << result << '\n';
    }
    catch (...)
    {
      in_.setstate(in_.failbit);
    }
  }
}

semichev::Peek::Peek(Spreadsheet& ws, std::istream& in, std::ostream& out):
  ws_(ws),
  in_(in),
  out_(out)
{}

void semichev::Peek::operator()()
{
  if (ws_.isEmpty())
  {
    in_.setstate(in_.failbit);
    return;
  }

  std::string str;
  if (!(in_ >> str))
  {
    return;
  }

  try
  {
    if (str == "sheet")
    {
      in_ >> str;
      operator()(str);
    }
    else
    {
      auto cell = toExprElem(str).getAddress().getCell();
      out_ << ws_.getCurrent().at(cell.first).at(cell.second).getFunction() << '\n';
    }
  }
  catch (...)
  {
    in_.setstate(in_.failbit);
  }
}

void semichev::Peek::operator()(const std::string& str)
{
  ws_.setCurrent(str);
  for (auto i = ws_.getCurrent().begin(); i != ws_.getCurrent().end(); i++)
  {
    for (auto j = i->second.begin(); j != i->second.end(); ++j)
    {
      out_ << j->first << ":" << i->first << " " << j->second.getFunction() << "\n";
    }
  }
}

semichev::Copy::Copy(Spreadsheet& ws, std::istream& in):
  ws_(ws),
  in_(in)
{}

void semichev::Copy::operator()()
{
  if (ws_.isEmpty())
  {
    in_.setstate(in_.failbit);
    return;
  }

  std::string str;
  if (!(in_ >> str))
  {
    return;
  }

  try
  {
    if (str == "range")
    {
      std::string cells[2];
      in_ >> cells[0] >> cells[1];
      auto from = toExprElem(cells[0]).getAddress().getCell();
      auto to = toExprElem(cells[1]).getAddress().getCell();
      ws_.copy(from, to);
    }
    else
    {

      auto cell = toExprElem(str).getAddress().getCell();
      ws_.copy(cell);
    }
  }
  catch (...)
  {
    in_.setstate(in_.failbit);
  }
}

semichev::Paste::Paste(Spreadsheet& ws, std::istream& in):
  ws_(ws),
  in_(in)
{}

void semichev::Paste::operator()()
{
  if (ws_.isEmpty())
  {
    in_.setstate(in_.failbit);
    return;
  }

  std::string str;
  if (!(in_ >> str))
  {
    return;
  }

  try
  {
    if (str == "range")
    {
      std::string cells[2];
      in_ >> cells[0] >> cells[1];
      auto from = toExprElem(cells[0]).getAddress().getCell();
      auto to = toExprElem(cells[1]).getAddress().getCell();
      ws_.paste(from, to);
    }
    else
    {

      auto cell = toExprElem(str).getAddress().getCell();
      ws_.paste(cell);
    }
  }
  catch (...)
  {
    in_.setstate(in_.failbit);
  }
}
