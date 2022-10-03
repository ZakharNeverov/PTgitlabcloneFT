#include "parser.hpp"

#include <iostream>
#include <fstream>

std::istream& alhimenko::skipblank(std::istream& in)
{
  while (std::isblank(in.peek()))
  {
    in.ignore();
  }
  return in;
}

void alhimenko::skipcommand(std::istream& in)
{
  std::cerr << "\n<INCORRECT COMMAND>\n";
  in.clear();
  in.ignore(std::numeric_limits< std::streamsize >::max(), ';');
  in.ignore();
}

alhimenko::SkipWsFlagSaver::SkipWsFlagSaver(std::istream& in)
{
  if (in.flags() & std::ios_base::skipws)
  {
    worked_ = true;
  }
  std::noskipws(in);
  in_ = &in;
}

alhimenko::SkipWsFlagSaver::~SkipWsFlagSaver()
{
  if (worked_)
  {
    std::skipws(*in_);
  }
}
