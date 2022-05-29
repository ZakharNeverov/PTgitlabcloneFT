#ifndef COMMANDS_WRAPPER_HPP
#define COMMANDS_WRAPPER_HPP

#include <iostream>

#include "commands.h"
#include "dictionary_usings.h"

namespace rakitin {
  void executeAdd(dictionaries& dicts, std::ostream& out, std::istream& in);
  void executeFindWord(const dictionaries& dicts, std::ostream& out, std::istream& in);
  void executePrintAll(const dictionaries& dicts, std::ostream& out, std::istream& in);
  void executeMerge(dictionaries& dicts, std::ostream& out, std::istream& in);
  void executeClear(dictionaries& dicts, std::ostream& out, std::istream& in);
  void executeIntersection(dictionaries& dicts, std::ostream& out, std::istream& in);
  void executeSimilarity(const dictionaries& dicts, std::ostream& out, std::istream& in);
  void executeRename(dictionaries& dicts, std::ostream& out, std::istream& in);
  void executeDelete(dictionaries& dicts, std::ostream& out, std::istream& in);
  void executePrintAllDatanames(const dictionaries& dicts, std::ostream& out);

}

#endif
