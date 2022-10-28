#ifndef COMMANDS_H
#define COMMANDS_H
#include <iosfwd>
#include "huffman.h"

namespace zozulya
{
  void readFileCMD(std::istream& in);
  void fillFrequencyCMD(std::istream& in);
  void printCodeDifferenceCMD(std::istream& in);
  void toCode(std::istream& in);
  void compressionFiles(std::istream& in);
  void compressionAllFiles(std::istream& in);
  void delta(std::istream& in);
  void isFileEncrypted(std::istream& in);

  std::string getMsg(std::string& message);
}
#endif
