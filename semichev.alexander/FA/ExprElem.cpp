#include "ExprElem.hpp"

#include <iostream>
#include <stdexcept>

#include "Spreadsheet.hpp"

namespace smcv = semichev;

semichev::ExprElem::Operand::Operand(double val):
  val_(val)
{}

double semichev::ExprElem::Operand::getValue() const
{
  return val_;
}

semichev::ExprElem::Address::Address(int column, int row):
  column_(column),
  row_(row)
{}

double semichev::ExprElem::Address::getValue(Spreadsheet& ws, const std::pair< int, int >& offset) const
{
  return ws.getCurrent()[row_ + offset.first][column_ + offset.second].getValue(ws);
}

std::pair< int, int > semichev::ExprElem::Address::getCell() const
{
  return std::pair< int, int >(row_, column_);
}

semichev::ExprElem::Operation::Operation(char type, int priority):
  type_(type),
  priority_(priority)
{}

smcv::ExprElem::Operation semichev::ExprElem::Operation::getAdd()
{
  return Operation('+', 1);
}

smcv::ExprElem::Operation semichev::ExprElem::Operation::getSub()
{
  return Operation('-', 1);
}

smcv::ExprElem::Operation semichev::ExprElem::Operation::getMul()
{
  return Operation('*', 0);
}

smcv::ExprElem::Operation semichev::ExprElem::Operation::getDiv()
{
  return Operation('/', 0);
}

char semichev::ExprElem::Operation::getType() const
{
  return type_;
}

bool semichev::ExprElem::Operation::hasMorePriority(const Operation& other) const
{
  return priority_ < other.priority_;
}

semichev::ExprElem::Parenthesis::Parenthesis(char type):
  type_(type)
{}

smcv::ExprElem::Parenthesis semichev::ExprElem::Parenthesis::getLeft()
{
  return Parenthesis('(');
}

smcv::ExprElem::Parenthesis semichev::ExprElem::Parenthesis::getRight()
{
  return Parenthesis(')');
}

char semichev::ExprElem::Parenthesis::getType() const
{
  return type_;
}

semichev::ExprElem::ExprElem(const Operation& val):
  type_(Type::OPERATION),
  operation_(val)
{}

semichev::ExprElem::ExprElem(const Operand& val):
  type_(Type::OPERAND),
  operand_(val)
{}

semichev::ExprElem::ExprElem(const Address& val):
  type_(Type::ADDRESS),
  address_(val)
{}

semichev::ExprElem::ExprElem(const Parenthesis& val):
  type_(Type::PARENTHESIS),
  parenthesis_(val)
{}

smcv::ExprElem::Type semichev::ExprElem::getType() const
{
  return type_;
}

smcv::ExprElem::Operand semichev::ExprElem::getOperand() const
{
  if (type_ != Type::OPERAND)
  {
    throw std::logic_error("type is not operand");
  }
  return operand_;
}

smcv::ExprElem::Address semichev::ExprElem::getAddress() const
{
  if (type_ != Type::ADDRESS)
  {
    throw std::logic_error("type is not operand");
  }
  return address_;
}

smcv::ExprElem::Operation semichev::ExprElem::getOperation() const
{
  if (type_ != Type::OPERATION)
  {
    throw std::logic_error("type is not opertion");
  }
  return operation_;
}

smcv::ExprElem::Parenthesis semichev::ExprElem::getParenthesis() const
{
  if (type_ != Type::PARENTHESIS)
  {
    throw std::logic_error("type is not parenthesis");
  }
  return parenthesis_;
}

smcv::ExprElem semichev::toExprElem(const std::string& str)
{
  ExprElem elem(ExprElem::Operand(0));

  if (str.size() == 1)
  {
    switch (str[0])
    {
    case '(':
      elem = ExprElem(ExprElem::Parenthesis::getLeft());
      break;
    case ')':
      elem = ExprElem(ExprElem::Parenthesis::getRight());
      break;
    case '+':
      elem = ExprElem(ExprElem::Operation::getAdd());
      break;
    case '-':
      elem = ExprElem(ExprElem::Operation::getSub());
      break;
    case '*':
      elem = ExprElem(ExprElem::Operation::getMul());
      break;
    case '/':
      elem = ExprElem(ExprElem::Operation::getDiv());
      break;
    default:
      elem = ExprElem(ExprElem::Operand(std::stod(str)));
      break;
    }
  }
  else
  {
    size_t n = str.find(":");
    if (n != str.npos)
    {
      int column = std::stoi(str.substr(0, n));
      int row = std::stoi(str.substr(n + 1));
      elem = ExprElem(ExprElem::Address(column, row));
    }
    else
    {
      elem = ExprElem(ExprElem::Operand(std::stod(str)));
    }
  }

  return elem;
}
