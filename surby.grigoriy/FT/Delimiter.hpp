#ifndef DELIMITER_HPP
#define DELIMITER_HPP
#include <iosfwd>
#include <string>

struct DelimiterSimv
{
  char simv;
};

std::istream& operator>>(std::istream& in, DelimiterSimv&& value);



std::string getNextArg(std::string&, size_t);

#endif











