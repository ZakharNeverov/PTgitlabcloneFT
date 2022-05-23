#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <set>
#include <map>
#include <list>
#include <functional>
#include <iosfwd>

namespace emelyanov {
  using CommandArgs = std::list< std::string >;
  using DataSet = std::map< std::string, std::set< size_t > >;
  using AllDataSets = std::map< std::string, DataSet >;
  using Commands = std::map< std::string, std::function< void(CommandArgs&) > >;

  class Command {
  public:
    Command() = default;

    Command(AllDataSets& data, std::istream& in, std::ostream& out);

    ~Command() = default;

    void operator()(CommandArgs& args);

    void read(CommandArgs& args);
    void printDictionary(CommandArgs& args);
    void printWord(CommandArgs& args);
    void complementDict(CommandArgs& args);
    void intersectDict(CommandArgs& args);
    void unionDict(CommandArgs& args);
    void renameDict(CommandArgs& args);
    void deleteDict(CommandArgs& args);
    void contains(CommandArgs& args);

  private:
    AllDataSets& dataSets_;
    std::istream& in_;
    std::ostream& out_;
    Commands mapOfCommands_;

    void doRead(CommandArgs& args);
    void doPrintDictionary(CommandArgs& args);
    void printWordFromOneDict(CommandArgs& args);
    void printWordFromAllDicts(CommandArgs& args);
    void doComplementDict(CommandArgs& args);
    void doIntersectDict(CommandArgs& args);
    void doUnionDict(CommandArgs& args);
    void doRenameDict(CommandArgs& args);
    void doDeleteDict(CommandArgs& args);
    void doContains(CommandArgs& args);
  };
}

#endif