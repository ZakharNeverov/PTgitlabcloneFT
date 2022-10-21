#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <deque>
#include <map>
#include <string>
namespace matyushenko
{
  class Commands
  {
  public:
    using StrT = std::string;
    using StrsDictT = std::map< StrT, StrT >;
    using ListOfArgs = std::deque< StrT >;
    Commands(StrsDictT& strs, std::ostream& out);
    void call(const StrT& commandName, const ListOfArgs& args) const noexcept;
  
  private:
    using CommandT = void (*)(StrsDictT&, const ListOfArgs&);
    using ConstCommandT = void (*)(const StrsDictT&, const ListOfArgs&, std::ostream&);
    using CommandsDictT = std::map< StrT, CommandT >;
    using ConstCommandsDictT = std::map< StrT, ConstCommandT >;
    StrsDictT& strs_;
    const CommandsDictT commands_;
    const ConstCommandsDictT constCommands_;
    std::ostream& out_;
  };
}
#endif
