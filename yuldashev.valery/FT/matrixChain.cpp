#include "matrixChain.hpp"
namespace yuldashev
{
  bool existName(const std::string& name, const MatrixChain& matrixChain)
  {
    return matrixChain.dChain.find(name) != matrixChain.dChain.end() || matrixChain.iChain.find(name) != matrixChain.iChain.end();
  }
}
