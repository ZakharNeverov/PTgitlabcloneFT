#include "IOfile.hpp"
#include <stdexcept>
#include <fstream>

std::ifstream gusarov::openFile(std::string name)
{
  std::ifstream file(name);
  if (!file.is_open()) {
    throw std::invalid_argument("File cannot open");
  } else if (file.peek() == EOF) {
    throw std::invalid_argument("File is empty");
  }
  return file;
}

std::string gusarov::getSymbol(char ch)
{
  std::string out;
  if (ch == '\n') {
    out = "\\n";
  } else if (ch == '\t') {
    out = "\\t";
  } else if (ch == ' ') {
    out = "\\s";
  } else {
    out = ch;
  }
  return out;
}

char gusarov::getSymbol(std::string str)
{
  char ch = 0;
  if (str == "\\n") {
    ch = '\n';
  } else if (str == "\\t") {
    ch = '\t';
  } else if (str == "\\s") {
    ch = ' ';
  } else if (str.size() > 1) {
    throw std::invalid_argument("Incorrect code");
  } else {
    ch = str[0];
  }
  return ch;
}

std::istream& gusarov::operator>>(std::istream& in, nameFileIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::string input;
  in >> input;
  if (input.rfind(".txt") != input.size() - 4) {
    in.setstate(std::ios::failbit);
  } else {
    dest.ref = input;
  }
  return in;
}
