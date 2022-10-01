#include "print.hpp"
#include <iostream>
namespace yuldashev
{
  std::ostream& printInvalid(std::ostream& out)
  {
    out << "<INVALID COMMAND>\n";
    return out;
  }
}
