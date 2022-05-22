#ifndef COMMAND_SYSTEM_HPP
#define COMMAND_SYSTEM_HPP

#include <iosfwd>
#include <string>
#include <vector>
#include <functional>
#include <map>

namespace gusarov {
  class Command
  {
  public:
    Command();
    void encode(std::istream&);
    void decode(std::istream&);
    void getCodeFano(std::istream&);
    void printFrequency(std::istream&);
    void mergeTwoFiles(std::istream&);
    void print(std::istream&);
    void compareSize(std::istream&);
    void doCommand(std::string);
  private:
    std::map< std::string, std::function< void() > > commandList;
  };
}
#endif
