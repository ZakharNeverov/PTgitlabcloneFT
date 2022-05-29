#ifndef COMMAND_WRAPPERS_H
#define COMMAND_WRAPPERS_H
#include "Commands.h"
#include <map>
#include <list>

namespace borisov
{
  using AllDicts = std::map< std::string, Dict >;
  using ArgList = std::list< std::string >;

  void addWrapper(AllDicts& allDicts, const ArgList& argList);
  void removeWrapper(AllDicts& allDicts, const ArgList& argList);
  void manualWrapper(AllDicts& allDicts, const ArgList& argList);
  void wordsWrapper(std::ostream& out, AllDicts& allDicts, const ArgList& argList);
  void infoWrapper(std::ostream& out, AllDicts& allDicts, const ArgList& argList);
  void translateWrapper(std::ostream& out, AllDicts& allDicts, const ArgList& argList);
  void saveWrapper(AllDicts& allDicts, const ArgList& argList);
  void uniteWrapper(AllDicts& allDicts, const ArgList& argList);
  void intersectWrapper(AllDicts& allDicts, const ArgList& argList);
  void complementWrapper(AllDicts& allDicts, const ArgList& argList);
}
#endif
