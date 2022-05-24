#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <iosfwd>
#include <utility>

#include "ForwardDeclaration.hpp"

namespace semichev
{
  class Function
  {
   public:
    virtual double operator()(Spreadsheet& ws, const std::pair< int, int >& offset) const = 0;
    virtual std::istream& read(std::istream& in) = 0;
    virtual std::ostream& print(std::ostream& out, const std::pair< int, int >& offset) const = 0;
  };

  std::istream& operator>>(std::istream& in, Function& func);
  std::ostream& operator<<(std::ostream& out, const std::pair< std::pair< int, int >, Function& >& func);
}

#endif
