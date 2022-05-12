#ifndef COMMAND_WRAPPERS_H
#define COMMAND_WRAPPERS_H

#include "Commands.h"
namespace dudnik
{
  void printWrapper(Args&, AllDicts&);
  void complementWrapper(Args&, AllDicts&);
  void intersectWrapper(Args&, AllDicts&);
  void uniteWrapper(Args&, AllDicts&);
  void replaceWrapper(Args&, AllDicts&);
  void removeWrapper(Args&, AllDicts&);
  void equalWrapper(Args&, AllDicts&);
  void insertWrapper(Args&, AllDicts&);
  void clearWrapper(Args&, AllDicts&);
  void addWrapper(Args&, AllDicts&);
}

#endif
