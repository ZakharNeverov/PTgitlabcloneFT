#include "map-commands.hpp"
#include <iomanip>
#include <functional>
#include "commands.hpp"
#include "print-messages.hpp"
#include "iofmtguard.hpp"
void pyankov::create(pyankov::chain_t& chain, std::istream& in, int minValue, int maxValue, size_t precision)
{
  std::string type = "";
  in >> type;
  if (!in)
  {
    throw std::invalid_argument("Incorrect create parameters!");
  }
  std::map< std::string, std::function< void() > > createCommands(
    {
      {"int", std::bind(pyankov::doCreateIntMatrix, std::ref(chain), std::ref(in), minValue, maxValue, precision)},
      {"double", std::bind(pyankov::doCreateDoubleMatrix, std::ref(chain), std::ref(in), minValue, maxValue, precision)}
    }
  );
  createCommands.at(type)();
}
void pyankov::add(pyankov::chain_t& chain, std::istream& in)
{
  std::string resultName = "";
  in >> resultName;
  if (!in)
  {
    throw std::invalid_argument("Incorrect add arguments!");
  }
  std::vector< std::string > names;
  bool isDouble = false;
  while (in.peek() != '\n')
  {
    std::string name = "";
    in >> name;
    if (!in || !pyankov::isContainingName(chain, name))
    {
      throw std::invalid_argument("Incorrect add arguments!");
    }
    if (chain.second.count(name))
    {
      isDouble = true;
    }
    names.push_back(name);
  }
  if (names.size() < 2)
  {
    throw std::invalid_argument("Not enough matrices to add!");
  }
  size_t rows = 0;
  size_t columns = 0;
  if (chain.first.count(names.at(0)))
  {
    rows = chain.first.find(names.at(0))->second.getRows();
    columns = chain.first.find(names.at(0))->second.getColumns();
  }
  else
  {
    rows = chain.second.find(names.at(0))->second.getRows();
    columns = chain.second.find(names.at(0))->second.getColumns();
  }
  pyankov::Matrix< double > doubleMatrix(rows, columns);
  pyankov::Matrix< int > intMatrix(rows, columns);
  for (size_t i = 0; i < names.size(); i++)
  {
    if (isDouble && chain.first.count(names.at(i)))
    {
      doubleMatrix += chain.first.find(names.at(i))->second;
    }
    else if (isDouble && chain.second.count(names.at(i)))
    {
      doubleMatrix += chain.second.find(names.at(i))->second;
    }
    else
    {
      intMatrix += chain.first.find(names.at(i))->second;
    }
  }
  if (isDouble && chain.second.count(resultName) && !chain.first.count(resultName))
  {
    chain.second.find(resultName)->second = doubleMatrix;
  }
  else if (isDouble && !pyankov::isContainingName(chain, resultName))
  {
    chain.second.insert({resultName, doubleMatrix});
  }
  else if (!isDouble && chain.first.count(resultName) && !chain.second.count(resultName))
  {
    chain.first.find(resultName)->second = intMatrix;
  }
  else if (!isDouble && !pyankov::isContainingName(chain, resultName))
  {
    chain.first.insert({resultName, intMatrix});
  }
  else
  {
    throw std::logic_error("Incorrect add arguments!");
  }
}
void pyankov::sub(pyankov::chain_t& chain, std::istream& in)
{
  std::string resultName = "";
  in >> resultName;
  if (!in)
  {
    throw std::invalid_argument("Incorrect sub arguments!");
  }
  std::vector< std::string > names;
  bool isDouble = false;
  while (in.peek() != '\n')
  {
    std::string name = "";
    in >> name;
    if (!in || !pyankov::isContainingName(chain, name))
    {
      throw std::invalid_argument("Incorrect sub arguments!");
    }
    if (chain.second.count(name))
    {
      isDouble = true;
    }
    names.push_back(name);
  }
  if (names.size() < 2)
  {
    throw std::invalid_argument("Not enough matrices to sub!");
  }
  size_t rows = 0;
  size_t columns = 0;
  if (chain.first.count(names.at(0)))
  {
    rows = chain.first.find(names.at(0))->second.getRows();
    columns = chain.first.find(names.at(0))->second.getColumns();
  }
  else
  {
    rows = chain.second.find(names.at(0))->second.getRows();
    columns = chain.second.find(names.at(0))->second.getColumns();
  }
  pyankov::Matrix< double > doubleMatrix(rows, columns);
  pyankov::Matrix< int > intMatrix(rows, columns);
  if (chain.first.count(names.at(0)))
  {
    doubleMatrix += chain.first.find(names.at(0))->second;
    intMatrix += chain.first.find(names.at(0))->second;
  }
  else
  {
    doubleMatrix += chain.second.find(names.at(0))->second;
  }
  for (size_t i = 1; i < names.size(); i++)
  {
    if (isDouble && chain.first.count(names.at(i)))
    {
      doubleMatrix -= chain.first.find(names.at(i))->second;
    }
    else if (isDouble && chain.second.count(names.at(i)))
    {
      doubleMatrix -= chain.second.find(names.at(i))->second;
    }
    else
    {
      intMatrix -= chain.first.find(names.at(i))->second;
    }
  }
  if (isDouble && chain.second.count(resultName) && !chain.first.count(resultName))
  {
    chain.second.find(resultName)->second = doubleMatrix;
  }
  else if (isDouble && !pyankov::isContainingName(chain, resultName))
  {
    chain.second.insert({resultName, doubleMatrix});
  }
  else if (!isDouble && chain.first.count(resultName) && !chain.second.count(resultName))
  {
    chain.first.find(resultName)->second = intMatrix;
  }
  else if (!isDouble && !pyankov::isContainingName(chain, resultName))
  {
    chain.first.insert({resultName, intMatrix});
  }
  else
  {
    throw std::logic_error("Incorrect sub arguments!");
  }
}
void pyankov::multiply(pyankov::chain_t& chain, std::istream& in)
{
  std::string resultName = "";
  in >> resultName;
  if (!in)
  {
    throw std::invalid_argument("Incorrect multiply arguments!");
  }
  std::vector< std::string > names;
  bool isDouble = false;
  while (in.peek() != '\n')
  {
    std::string name = "";
    in >> name;
    if (!in || !pyankov::isContainingName(chain, name))
    {
      throw std::invalid_argument("Incorrect multiply arguments!");
    }
    if (chain.second.count(name))
    {
      isDouble = true;
    }
    names.push_back(name);
  }
  if (names.size() < 2)
  {
    throw std::invalid_argument("Not enough matrices to multiply!");
  }
  size_t rows = 0;
  size_t columns = 0;
  if (chain.first.count(names.at(0)))
  {
    rows = chain.first.find(names.at(0))->second.getRows();
    columns = chain.first.find(names.at(0))->second.getColumns();
  }
  else
  {
    rows = chain.second.find(names.at(0))->second.getRows();
    columns = chain.second.find(names.at(0))->second.getColumns();
  }
  pyankov::Matrix< double > doubleMatrix(rows, columns);
  pyankov::Matrix< int > intMatrix(rows, columns);
  if (chain.first.count(names.at(0)))
  {
    doubleMatrix += chain.first.find(names.at(0))->second;
    intMatrix += chain.first.find(names.at(0))->second;
  }
  else
  {
    doubleMatrix += chain.second.find(names.at(0))->second;
  }
  for (size_t i = 1; i < names.size(); i++)
  {
    if (isDouble && chain.first.count(names.at(i)))
    {
      doubleMatrix *= chain.first.find(names.at(i))->second;
    }
    else if (isDouble && chain.second.count(names.at(i)))
    {
      doubleMatrix *= chain.second.find(names.at(i))->second;
    }
    else
    {
      intMatrix *= chain.first.find(names.at(i))->second;
    }
  }
  if (isDouble && chain.second.count(resultName) && !chain.first.count(resultName))
  {
    chain.second.find(resultName)->second = doubleMatrix;
  }
  else if (isDouble && !pyankov::isContainingName(chain, resultName))
  {
    chain.second.insert({resultName, doubleMatrix});
  }
  else if (!isDouble && chain.first.count(resultName) && !chain.second.count(resultName))
  {
    chain.first.find(resultName)->second = intMatrix;
  }
  else if (!isDouble && !pyankov::isContainingName(chain, resultName))
  {
    chain.first.insert({resultName, intMatrix});
  }
  else
  {
    throw std::logic_error("Incorrect multiply arguments!");
  }
}
void pyankov::concat(pyankov::chain_t& chain, std::istream& in)
{
  std::string direction = "";
  std::string resultName = "";
  std::string leftName = "";
  std::string rightName = "";
  in >> direction >> resultName >> leftName >> rightName;
  if (!in || in.peek() != '\n' || !pyankov::isContainingName(chain, leftName) || !pyankov::isContainingName(chain, rightName))
  {
    throw std::invalid_argument("Incorrect concat parameters!");
  }
  std::map< std::string, std::function< void() > > concatCommands(
    {
      {"toright", std::bind(pyankov::concatToRight, std::ref(chain), leftName, rightName, resultName)},
      {"toleft", std::bind(pyankov::concatToRight, std::ref(chain), rightName, leftName, resultName)},
      {"totop", std::bind(pyankov::concatToBot, std::ref(chain), rightName, leftName, resultName)},
      {"tobot", std::bind(pyankov::concatToBot, std::ref(chain), leftName, rightName, resultName)},
    }
  );
  concatCommands.at(direction)();
}
void pyankov::print(const pyankov::chain_t& chain, std::istream& in, std::ostream& out, size_t precision)
{
  std::string name = "";
  in >> name;
  if (!in || !(chain.first.count(name) || chain.second.count(name)))
  {
    throw std::invalid_argument("Incorrect print command!");
  }
  if (chain.first.count(name))
  {
    out << chain.first.find(name)->second;
  }
  else
  {
    pyankov::ioFmtGuard guard(out);
    out << std::setprecision(precision);
    out << chain.second.find(name)->second;
  }
}
void pyankov::equals(const pyankov::chain_t& chain, std::istream& in, std::ostream& out)
{
  std::string prevName = "";
  in >> prevName;
  if (!in || in.peek() == '\n' || !pyankov::isContainingName(chain, prevName))
  {
    throw std::logic_error("Incorrect equals argument!");
  }
  bool isEqual = true;
  while (in.peek() != '\n')
  {
    std::string currName = "";
    in >> currName;
    if (!in || !pyankov::isContainingName(chain, currName))
    {
      throw std::logic_error("Incorrect equals argument!");
    }
    isEqual = pyankov::compareMatricesByName(chain, prevName, currName);
    if (!isEqual)
    {
      break;
    }
    prevName = currName;
  }
  if (isEqual)
  {
    pyankov::printTrue(out);
  }
  else
  {
    pyankov::printFalse(out);
  }
}
