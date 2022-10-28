#include "ioStruct.h"
#include <stdexcept>
#include <fstream>

std::ifstream varnavskii::openFile(const std::string& name)
{
  std::ifstream file(name);
  if (!file.is_open())
  {
    throw std::invalid_argument("File not open");
  } else if (file.peek() == EOF)
  {
    throw std::invalid_argument("File is empty");
  }
  return file;
}

std::string varnavskii::getSymbol(char ch)
{
  std::string out;
  if (ch == '\n')
  {
    out = "\\n";
  } else if (ch == '\t')
  {
    out = "\\t";
  } else if (ch == ' ')
  {
    out = "\\s";
  }
  else
  {
    out = ch;
  }
  return out;
}

std::istream& varnavskii::operator>>(std::istream& in, fileIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string input;
  in >> input;
  if (input.rfind(".txt") != input.size() - 4)
  {
    in.setstate(std::ios::failbit);
  }
  else
  {
    dest.ref = input;
  }
  return in;
}
