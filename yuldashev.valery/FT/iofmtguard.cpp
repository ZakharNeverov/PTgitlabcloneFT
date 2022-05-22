#include "iofmtguard.hpp"
#include <iostream>
namespace yuldashev
{
  iofmtguard::iofmtguard(std::basic_ios< char >& s) :
    s_(s),
    fill_(s.fill()),
    prec_(s.precision()),
    flags_(s.flags())
  {}
  iofmtguard::~iofmtguard()
  {
    s_.fill(fill_);
    s_.precision(prec_);
    s_.flags(flags_);
  }
}
