#ifndef COMMAND_SYSTEM_HPP
#define COMMAND_SYSTEM_HPP

#include <string>
#include <functional>
#include <map>

namespace gusarov {
  using dict_t = std::map< char, std::string >;

  class Command
  {
  public:
    Command();
    void add(std::istream&);
    void code(std::istream&);
    void encode(std::istream&);
    void decode(std::istream&);
    void frequency(std::istream&, std::ostream&);
    void print(std::istream&, std::ostream&);
    void compare(std::istream&, std::ostream&);
    void merge(std::istream&);
    void doCommand(std::string);
  private:
    std::map< std::string, std::function< void() > > commandList_;
    std::map< std::string, std::string > texts_;
    std::map< std::string, dict_t > dictionaries_;
  };
}
#endif
