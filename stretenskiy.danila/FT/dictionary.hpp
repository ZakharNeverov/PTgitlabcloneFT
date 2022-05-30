#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <string>
#include <unordered_map>
#include "ioProccessing.hpp"

namespace stretenskiy
{
  using content = std::unordered_map< std::string, std::set < std::string > >;
  using transl = std::set < std::string >;

  struct Dictionary
  {
    std::string name_;
    content content_;
  };

  std::istream &operator>>(std::istream &, Dictionary &);
  std::ostream &operator<<(std::ostream &, const transl &);
  std::ostream &operator<<(std::ostream &, const content &);
  std::ostream &operator<<(std::ostream &, const Dictionary &);
  std::ostream &commandPrintDict(std::ostream &, const Dictionary &);
}

#endif
