#ifndef S3_COMMANDMAKER_H
#define S3_COMMANDMAKER_H

#include <string>
#include <functional>
#include <map>
#include <list>

namespace bendryshev
{
  std::list< std::string > split(const std::string&);
  std::list< std::string > readListFromStream(std::istream&);

  class CommandMaker
  {
  public:
    using command = std::string;
    using value_t = int;
    using command_list_iterator = std::list< command >::iterator;
    using commandsAction = std::function< void(command_list_iterator, command_list_iterator) >;
    using cmd_dict = std::map< command, commandsAction, std::less< > >;
    using list_dict = std::map< std::string, std::list< value_t >, std::less< > >;
    struct pos
    {
      value_t index_;
      std::list< value_t > list_;
      std::list< value_t >::iterator iter_;
    };
    using pos_dict = std::map< std::string, pos, std::less< > >;
    using list = std::list< value_t >;
    explicit CommandMaker(std::ostream&);
    void readLists(std::istream&);
    void doCommand(std::list< command >&);
  private:
    std::ostream& out_;
    list_dict lists_;
    pos_dict positions_;
    cmd_dict command_dictionary_;
    void doPrintCommand(command_list_iterator, command_list_iterator);
    void doReplaceCommand(command_list_iterator, command_list_iterator);
    void doRemoveCommand(command_list_iterator, command_list_iterator);
    void doConcatCommand(command_list_iterator, command_list_iterator);
    void doEqualCommand(command_list_iterator, command_list_iterator);
    void doBeginCommand(command_list_iterator, command_list_iterator);
    void doEndCommand(command_list_iterator, command_list_iterator);
    void doMismatchCommand(command_list_iterator, command_list_iterator);
    void doPrintPosCommand(command_list_iterator, command_list_iterator);
    void doSwapCommand(command_list_iterator, command_list_iterator);
    void doFindCommand(command_list_iterator, command_list_iterator);
    void doRotateCommand(command_list_iterator, command_list_iterator);
    void doSearchCommand(command_list_iterator, command_list_iterator);
  };
}

#endif
