#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <vector>
#include "matrixChain.hpp"
namespace yuldashev
{
  void doCreate(std::istream& in, MatrixChain& chain);
  void doImport(std::istream& in, MatrixChain& chain);
  void doMultiply(std::istream& in, MatrixChain& chain);
  void doPrint(std::istream& in, std::ostream& out, const MatrixChain& chain, bool isFile);
  void doDelete(std::istream& in, MatrixChain& chain);
  void doSave(std::istream& in, MatrixChain& chain);
  void doReplaceAt(std::istream& in, MatrixChain& chain);
  void doSum(std::istream& in, MatrixChain& chain);
  void doMultiplyByElement(std::istream& in, MatrixChain& chain);
}
#endif
