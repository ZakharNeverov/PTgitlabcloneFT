#include "commands.hpp"
#include <fstream>
#include <algorithm>
#include <map>
#include "matrixChain.hpp"
#include "operations.hpp"
#include "print.hpp"
namespace yuldashev
{
  void doCreate(std::istream& in, MatrixChain& chain)
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
      Matrix< int > matrix;
      setDataToMatrix(in, matrix);
      chain.iChain.insert({ name, matrix });
    }
    else if (type == "double")
    {
      Matrix< double > matrix;
      setDataToMatrix(in, matrix);
      chain.dChain.insert({ name, matrix });
    }
    else
    {
      throw std::logic_error("Wrong type.");
    }
  }
  void doImport(std::istream& in, MatrixChain& chain)
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
      Matrix< int > matrix;
      setDataToMatrix(file, matrix);
      chain.iChain.insert({ name, matrix });
    }
    else if (type == "double")
    {
      Matrix< double > matrix;
      setDataToMatrix(file, matrix);
      chain.dChain.insert({ name, matrix });
    }
    else
    {
      throw std::logic_error("Wrong type.");
    }
  }
  void doMultiply(std::istream& in, MatrixChain& chain)
  {
    using namespace std::placeholders;
    doMathOperation< double >(in, chain, std::bind(multiply< double >, _1, _2));
  }
  void doPrint(std::istream& in, std::ostream& out, const MatrixChain& chain, bool isFile)
  {
    std::string name = "";
    in >> name;
    using namespace std::placeholders;
    bool res1 = findAndDo< int >(name, chain.iChain, std::bind(print< int >, std::ref(out), _1, isFile));
    bool res2 = findAndDo< double >(name, chain.dChain, std::bind(print< double >, std::ref(out), _1, isFile));
    if (!(res1 || res2))
    {
      throw std::logic_error("Wrong argument");
    }
  }
  void doDelete(std::istream& in, MatrixChain& chain)
  {
    std::string name = "";
    in >> name;
    using namespace std::placeholders;
    bool res1 = findAndDo< int >(name, chain.iChain, std::bind(deleteMatrix< int >, _1, std::ref(chain.iChain)));
    bool res2 = findAndDo< double >(name, chain.dChain, std::bind(deleteMatrix< double >, _1, std::ref(chain.dChain)));
    if (!(res1 || res2))
    {
      throw std::logic_error("Wrong argument");
    }
  }
  void doSave(std::istream& in, MatrixChain& chain)
  {
    std::string filename = "";
    in >> filename;
    if (filename.empty())
    {
      throw std::logic_error("Wrong filename.");
    }
    std::ofstream file(filename);
    if (!file.is_open())
    {
      throw std::logic_error("File not found.");
    }
    doPrint(in, file, chain, true);
    file.close();
  }
  void doReplaceAt(std::istream& in, MatrixChain& chain)
  {
    std::string name = "";
    size_t row = 0;
    size_t column = 0;
    double number = 0.0;
    in >> name >> row >> column >> number;
    if (name.empty() || in.rdstate() == std::ios::failbit)
    {
      throw std::logic_error("Wrong argument.");
    }
    using namespace std::placeholders;
    auto fInt = std::bind(replace< int >, _1, std::ref(chain.iChain), name, row, column, number);
    auto fDouble = std::bind(replace< double >, _1, std::ref(chain.dChain), name, row, column, number);
    bool res1 = findAndDo< int >(name, chain.iChain, fInt);
    bool res2 = findAndDo< double >(name, chain.dChain, fDouble);
    if (!(res1 || res2))
    {
      throw std::logic_error("Wrong argument");
    }
  }
  void doSum(std::istream& in, MatrixChain& chain)
  {
    using namespace std::placeholders;
    doMathOperation< double >(in, chain, std::bind(sum< double >, _1, _2));
  }
  void doMultiplyByElement(std::istream& in, MatrixChain& chain)
  {
    using namespace std::placeholders;
    doMathOperation< double >(in, chain, std::bind(multiplyByElement< double >, _1, _2));
  }
}
