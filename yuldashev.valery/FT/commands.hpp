#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <vector>
#include "matrixChain.hpp"
namespace yuldashev
{
  bool doCreate(std::istream& in, MatrixChain& chain);
  bool doImport(std::istream& in, MatrixChain& chain);
  bool doMultiply(std::istream& in, MatrixChain& chain);
  bool doPrint(std::istream& in, std::ostream& out, const MatrixChain& chain, bool isFile);
  bool doDelete(std::istream& in, MatrixChain& chain);
  bool doSave(std::istream& in, MatrixChain& chain);
  bool doReplaceAt(std::istream& in, MatrixChain& chain);
  bool doSum(std::istream& in, MatrixChain& chain);
  bool doMultiplyByElement(std::istream& in, MatrixChain& chain);
}
#endif
