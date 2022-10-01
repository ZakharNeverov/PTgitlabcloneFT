#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <map>
#include <deque>

#include "HaffmanStringUnion.hpp"

namespace malakhov
{
  class Commands
  {
  public:
    using StrT = std::string;
    using StrsDictT = std::map< StrT, HaffmanStringUnion >;
    using ArgsListT = std::deque< StrT >;

    Commands(StrsDictT& strs, std::ostream& out);
    void call(const StrT& commandName, const ArgsListT& args) const noexcept;

  private:
    using CommandT = void (*)(StrsDictT&, const ArgsListT&);
    using ConstCommandT = void (*)(const StrsDictT&, const ArgsListT&, std::ostream&);

    using CommandsDictT = std::map< StrT, CommandT >;
    using ConstCommandsDictT = std::map< StrT, ConstCommandT >;

    StrsDictT& strs_;
    const CommandsDictT commands_;
    const ConstCommandsDictT constCommands_;
    std::ostream& out_;
  };
}

#endif
