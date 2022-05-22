#include "commands.hpp"
#include <fstream>
#include <algorithm>
#include <map>
#include "matrixChain.hpp"
#include "operations.hpp"
#include "print.hpp"
namespace yuldashev
{
  bool doCreate(std::istream& in, MatrixChain& chain)
  {
    std::string name = "";
    in >> name;
    if (name.empty() || existName(name, chain))
    {
      throw std::logic_error("Wrong argument.");
    }
    std::string type = "";
    in >> type;
    if (type == "int")
    {
      Matrix<int> matrix;
      if (!setDataToMatrix(in, matrix))
      {
        return false;
      }
      chain.iChain.insert({ name, matrix });
      return true;
    }
    else if (type == "double")
    {
      Matrix<double> matrix;
      if (!setDataToMatrix(in, matrix))
      {
        return false;
      }
      chain.dChain.insert({ name, matrix });
      return true;
    }
    else
    {
      throw std::logic_error("Wrong type.");
    }
  }
  bool doImport(std::istream& in, MatrixChain& chain)
  {
    std::string filename = "";
    std::string name = "";
    std::string type = "";
    in >> filename >> name >> type;
    if (filename.empty() || name.empty() || type.empty() || existName(name, chain))
    {
      throw std::logic_error("Wrong argument.");
    }
    std::ifstream file(filename);
    if (!file.is_open())
    {
      throw std::logic_error("File not found.");
    }
    if (type == "int")
    {
      Matrix<int> matrix;
      if (!setDataToMatrix(file, matrix))
      {
        return false;
      }
      chain.iChain.insert({ name, matrix });
      return true;
    }
    else if (type == "double")
    {
      Matrix<double> matrix;
      if (!setDataToMatrix(file, matrix))
      {
        return false;
      }
      chain.dChain.insert({ name, matrix });
      return true;
    }
    else
    {
      throw std::logic_error("Wrong type.");
    }
    return true;
  }
  bool doMultiply(std::istream& in, MatrixChain& chain)
  {
    try
    {
      using namespace std::placeholders;
      return doMathOperation< double >(in, chain, std::bind(multiply< double >, _1, _2));
    }
    catch (...)
    {
      return false;
    }
  }
  bool doPrint(std::istream& in, std::ostream& out, const MatrixChain& chain, bool isFile)
  {
    std::string name = "";
    in >> name;
    using namespace std::placeholders;
    bool res1 = findAndDo< int >(name, chain.iChain, std::bind(print< int >, std::ref(out), _1, isFile));
    bool res2 = findAndDo< double >(name, chain.dChain, std::bind(print< double >, std::ref(out), _1, isFile));
    return res1 || res2;
  }
  bool doDelete(std::istream& in, MatrixChain& chain)
  {
    std::string name = "";
    in >> name;
    using namespace std::placeholders;
    bool res1 = findAndDo< int >(name, chain.iChain, std::bind(deleteMatrix< int >, _1, std::ref(chain.iChain)));
    bool res2 = findAndDo< double >(name, chain.dChain, std::bind(deleteMatrix< double >, _1, std::ref(chain.dChain)));
    return res1 || res2;
  }
  bool doSave(std::istream& in, MatrixChain& chain)
  {
    std::string filename = "";
    in >> filename;
    if (filename.empty())
    {
      throw "Wrong name/filename.";
    }
    std::ofstream file(filename);
    if (!file.is_open())
    {
      throw std::logic_error("File not found.");
    }
    if (!doPrint(in, file, chain, true))
    {
      file.close();
      return false;
    }
    file.close();
    return true;
  }
  bool doReplaceAt(std::istream& in, MatrixChain& chain)
  {
    std::string name = "";
    size_t row = 0;
    size_t column = 0;
    double number = 0.0;
    in >> name >> row >> column >> number;
    if (name.empty() || in.rdstate() == std::ios::failbit)
    {
      return false;
    }
    using namespace std::placeholders;
    auto fInt = std::bind(replace< int >, _1, std::ref(chain.iChain), name, row, column, number);
    auto fDouble = std::bind(replace< double >, _1, std::ref(chain.dChain), name, row, column, number);
    bool res1 = findAndDo< int >(name, chain.iChain, fInt);
    bool res2 = findAndDo< double >(name, chain.dChain, fDouble);
    return res1 || res2;
  }
  bool doSum(std::istream& in, MatrixChain& chain)
  {
    try
    {
      using namespace std::placeholders;
      return doMathOperation< double >(in, chain, std::bind(sum< double >, _1, _2));
    }
    catch (...)
    {
      return false;
    }
  }
  bool doMultiplyByElement(std::istream& in, MatrixChain& chain)
  {
    try
    {
      using namespace std::placeholders;
      return doMathOperation< double >(in, chain, std::bind(multiplyByElement< double >, _1, _2));
    }
    catch (...)
    {
      return false;
    }
  }
}
