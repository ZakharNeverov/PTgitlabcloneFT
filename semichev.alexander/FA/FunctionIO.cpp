#include "FunctionIO.hpp"
#include <iostream>
#include "Expression.hpp"
#include "Functions.hpp"

std::istream& semichev::operator>>(std::istream& in, FunctionIO& dest)
{
  std::string str;
  if (!(in >> str))
  {
    return in;
  }

  std::map< std::string, std::shared_ptr< Function > > funcs
  {
      { "=", std::make_shared<Expression>(0) },
      { "SUM", std::make_shared<Sum>(std::make_pair(0, 0), std::make_pair(0, 0)) },
      { "AVERAGE", std::make_shared<Average>(std::make_pair(0, 0), std::make_pair(0, 0)) },
      { "COUNT", std::make_shared<Count>(std::make_pair(0, 0), std::make_pair(0, 0), nullptr) },
      { "MIN", std::make_shared<Min>(std::make_pair(0, 0), std::make_pair(0, 0)) },
      { "MAX", std::make_shared<Max>(std::make_pair(0, 0), std::make_pair(0, 0)) },
  };

  try
  {
      if (funcs.find(str) != funcs.end())
      {
          if (in >> *funcs[str])
          {
              dest.func = funcs[str];
          }
      }
      else
      {
          dest.func = std::make_shared< Expression >(std::stod(str));
      }
  }
  catch (...)
  {
      in.setstate(in.failbit);
  }
  

  return in;
}