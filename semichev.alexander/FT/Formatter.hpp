#ifndef FORMATTERS_HPP
#define FORMATTERS_HPP

#include <ios>

namespace semichev
{
  class Formatter
  {
   public:
    Formatter() = delete;
    Formatter(const Formatter&) = delete;
    Formatter(std::ios_base& ios);
    ~Formatter();
    Formatter operator=(const Formatter&) = delete;

   private:
    std::ios_base& ios_;
    std::streamsize precision_;
    std::ios_base::fmtflags flags_;
  };
}

#endif
