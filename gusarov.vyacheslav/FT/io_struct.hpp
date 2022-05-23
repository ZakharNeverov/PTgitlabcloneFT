#ifndef IO_STRUCT_HPP
#define IO_STRUCT_HPP

#include <string>
#include "CommandSystem.hpp"

namespace gusarov {
  struct fileIO
  {
    std::string& ref;
  };

  std::ifstream openFile(const std::string&);
  std::string getSymbol(char);

  std::istream& operator>>(std::istream&, fileIO&&);
}
#endif
