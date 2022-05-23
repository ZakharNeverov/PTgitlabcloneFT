#include "Functions.hpp"

#include <iostream>
#include <limits>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <functional>

#include "ExprElem.hpp"
#include "FunctionIO.hpp"
#include "Spreadsheet.hpp"

namespace smcv = semichev;

namespace
{
  constexpr double MIN = std::numeric_limits< double >::min();
  constexpr double MAX = std::numeric_limits< double >::max();

  template< typename P >
  auto getPred(smcv::Spreadsheet& ws, P pred)
  {
    return [&](auto init, auto& cell)
    {
      return pred(init, cell.second.getValue(ws));
    };
  }

  template< typename T, typename P >
  double accumulate(T& c, std::pair< int, int > range, int offset, double init, P pred)
  {
    return std::accumulate(c.lower_bound(range.first + offset), c.upper_bound(range.second + offset), init, pred);
  }

  template< typename T, typename P >
  double count(T& c, std::pair< int, int > range, int offset, P pred)
  {
    return std::count_if(c.lower_bound(range.first + offset), c.upper_bound(range.second + offset), pred);
  }
}

semichev::RangeFunction::RangeFunction(const std::pair< int, int >& from, const std::pair< int, int >& to):
  rows_(std::minmax(from.first, to.first)),
  cols_(std::minmax(from.second, to.second))
{}

semichev::Sum::Sum(const std::pair< int, int >& from, const std::pair< int, int >& to):
  RangeFunction(from, to)
{}

double semichev::Sum::operator()(Spreadsheet& ws, const std::pair< int, int >& offset) const
{
  auto pred1 = getPred(ws, std::plus< double >());
  auto pred2 = [&](auto init, auto& row) -> double
  {
    return accumulate(row.second, cols_, offset.second, init, pred1);
  };
  return accumulate(ws.getCurrent(), rows_, offset.first, 0.0, pred2);
}

semichev::Average::Average(const std::pair< int, int >& from, const std::pair< int, int >& to):
  RangeFunction(from, to)
{}

double semichev::Average::operator()(Spreadsheet& ws, const std::pair< int, int >& offset) const
{
  auto pred1 = getPred(ws, std::plus< double >());
  auto pred2 = [&](auto init, auto& row) -> double
  {
    return accumulate(row.second, cols_, offset.second, init, pred1);
  };
  double sum = accumulate(ws.getCurrent(), rows_, offset.first, 0.0, pred2);
  return sum / ((rows_.second - rows_.first + 1) * (cols_.second - cols_.first + 1));
}

semichev::Count::Count(const std::pair< int, int >& from, const std::pair< int, int >& to, const std::shared_ptr< Function >& func)
  :RangeFunction(from, to), func_(func)
{}

double semichev::Count::operator()(Spreadsheet& ws, const std::pair< int, int >& offset) const
{
  double val = func_->operator()(ws, offset);
  auto pred1 = std::bind(getPred(ws, std::equal_to< double >()), val, std::placeholders::_1);
  auto pred2 = [&](auto init, auto& row) -> double
  {
    return init + count(row.second, cols_, offset.second, pred1);
  };
  return accumulate(ws.getCurrent(), rows_, offset.first, 0.0, pred2);
}

semichev::Min::Min(const std::pair< int, int >& from, const std::pair< int, int >& to):
  RangeFunction(from, to)
{}

double semichev::Min::operator()(Spreadsheet& ws, const std::pair< int, int >& offset) const
{
  auto pred1 = [&](auto init, auto& rhs) -> double
  {
    return std::min(init, rhs.second.getValue(ws));
  };
  auto pred2 = [&](auto init, auto& row) -> double
  {
    return std::min(init, accumulate(row.second, cols_, offset.second, MAX, pred1));
  };
  return accumulate(ws.getCurrent(), rows_, offset.first, MAX, pred2);
}

semichev::Max::Max(const std::pair< int, int >& from, const std::pair< int, int >& to):
  RangeFunction(from, to)
{}

double semichev::Max::operator()(Spreadsheet& ws, const std::pair< int, int >& offset) const
{
  auto pred1 = [&](auto init, auto& rhs) -> double
  {
    return std::max(init, rhs.second.getValue(ws));
  };
  auto pred2 = [&](auto init, auto& row) -> double
  {
    return std::max(init, accumulate(row.second, cols_, offset.second, MIN, pred1));
  };
  return accumulate(ws.getCurrent(), rows_, offset.first, MIN, pred2);
}

std::istream& semichev::Sum::read(std::istream& in)
{
  std::string args[2];
  if (in >> args[0] >> args[1])
  {
    try
    {
      *this = Sum(toExprElem(args[0]).getAddress().getCell(), toExprElem(args[1]).getAddress().getCell());
    }
    catch (...)
    {
      in.setstate(in.failbit);
    }
  }
  return in;
}

std::ostream& semichev::Sum::print(std::ostream& out, const std::pair< int, int >& offset) const
{
  out << "SUM ";
  out << cols_.first + offset.second << ":" << rows_.first + offset.first << " ";
  out << cols_.second + offset.second << ":" << rows_.second + offset.first;
  return out;
}

std::istream& semichev::Average::read(std::istream& in)
{
  std::string args[2];
  if (in >> args[0] >> args[1])
  {
    try
    {
      *this = Average(toExprElem(args[0]).getAddress().getCell(), toExprElem(args[1]).getAddress().getCell());
    }
    catch (...)
    {
      in.setstate(in.failbit);
    }
  }
  return in;
}

std::ostream& semichev::Average::print(std::ostream& out, const std::pair< int, int >& offset) const
{
  out << "AVERAGE ";
  out << cols_.first + offset.second << ":" << rows_.first + offset.first << " ";
  out << cols_.second + offset.second << ":" << rows_.second + offset.first;
  return out;
}

std::istream& semichev::Count::read(std::istream& in)
{
  std::string args[2];
  FunctionIO funcIO;
  if (in >> args[0] >> args[1] >> funcIO)
  {
    try
    {
      *this =
          Count(toExprElem(args[0]).getAddress().getCell(), toExprElem(args[1]).getAddress().getCell(), funcIO.func);
    }
    catch (...)
    {
      in.setstate(in.failbit);
    }
  }
  return in;
}

std::ostream& semichev::Count::print(std::ostream& out, const std::pair< int, int >& offset) const
{
  out << "COUNT ";
  out << cols_.first + offset.second << ":" << rows_.first + offset.first << " ";
  out << cols_.second + offset.second << ":" << rows_.second + offset.first << " ";
  return func_->print(out, offset);
}

std::istream& semichev::Min::read(std::istream& in)
{
  std::string args[2];
  if (in >> args[0] >> args[1])
  {
    try
    {
      *this = Min(toExprElem(args[0]).getAddress().getCell(), toExprElem(args[1]).getAddress().getCell());
    }
    catch (...)
    {
      in.setstate(in.failbit);
    }
  }
  return in;
}

std::ostream& semichev::Min::print(std::ostream& out, const std::pair< int, int >& offset) const
{
  out << "MIN ";
  out << cols_.first + offset.second << ":" << rows_.first + offset.first << " ";
  out << cols_.second + offset.second << ":" << rows_.second + offset.first;
  return out;
}

std::istream& semichev::Max::read(std::istream& in)
{
  std::string args[2];
  if (in >> args[0] >> args[1])
  {
    try
    {
      *this = Max(toExprElem(args[0]).getAddress().getCell(), toExprElem(args[1]).getAddress().getCell());
    }
    catch (...)
    {
      in.setstate(in.failbit);
    }
  }
  return in;
}

std::ostream& semichev::Max::print(std::ostream& out, const std::pair< int, int >& offset) const
{
  out << "MAX ";
  out << cols_.first + offset.second << ":" << rows_.first + offset.first << " ";
  out << cols_.second + offset.second << ":" << rows_.second + offset.first;
  return out;
}
