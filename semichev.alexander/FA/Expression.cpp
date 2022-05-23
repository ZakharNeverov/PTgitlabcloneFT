#include "Expression.hpp"

#include <iostream>
#include <string>
#include <stack>

namespace smcv = semichev;

namespace
{
  bool isOperation(const smcv::ExprElem& val)
  {
    return val.getType() == smcv::ExprElem::Type::OPERATION;
  }

  bool isOperand(const smcv::ExprElem& val)
  {
    return val.getType() == smcv::ExprElem::Type::OPERAND;
  }

  bool isAddress(const smcv::ExprElem& val)
  {
    return val.getType() == smcv::ExprElem::Type::ADDRESS;
  }

  bool isLeftParenthesis(const smcv::ExprElem& val)
  {
    return (val.getType() == smcv::ExprElem::Type::PARENTHESIS) && (val.getParenthesis().getType() == '(');
  }

  bool isRightParenthesis(const smcv::ExprElem& val)
  {
    return (val.getType() == smcv::ExprElem::Type::PARENTHESIS) && (val.getParenthesis().getType() == ')');
  }

  bool hasLessOrEqualPriority(const smcv::ExprElem& lhs, const smcv::ExprElem& rhs)
  {
    return isOperation(lhs) && isOperation(rhs) && !lhs.getOperation().hasMorePriority(rhs.getOperation());
  }

  smcv::Expression::expr_t convertToPostfix(smcv::Expression::expr_t&& infix)
  {
    smcv::Expression::expr_t postfix;
    std::stack< smcv::ExprElem > stack;

    while (!infix.empty())
    {
      try
      {
        if (isOperand(infix.front()) || isAddress(infix.front()))
        {
          postfix.push(infix.front());
        }
        else if (isLeftParenthesis(infix.front()))
        {
          stack.push(infix.front());
        }
        else if (isRightParenthesis(infix.front()))
        {
          while (!isLeftParenthesis(stack.top()))
          {
            postfix.push(stack.top());
            stack.pop();
          }
          stack.pop();
        }
        else
        {
          while (!stack.empty() && hasLessOrEqualPriority(infix.front(), stack.top()))
          {
            postfix.push(stack.top());
            stack.pop();
          }
          stack.push(infix.front());
        }
        infix.pop();
      }
      catch (...)
      {
        throw std::logic_error("expression is incorrect");
      }
    }

    while (!stack.empty())
    {
      if (!isOperation(stack.top()))
      {
        throw std::logic_error("expression is incorrect");
      }
      postfix.push(stack.top());
      stack.pop();
    }

    return postfix;
  }

  double calcOperation(char op, double lhs, double rhs)
  {
    double value = 0;
    switch (op)
    {
    case '+':
      value = lhs + rhs;
      break;
    case '-':
      value = lhs - rhs;
      break;
    case '*':
      value = lhs * rhs;
      break;
    case '/':
      value = lhs / rhs;
      break;
    default:
      break;
    }
    return value;
  }

  double calcPostfixExpr(smcv::Expression::expr_t&& postfix, smcv::Spreadsheet& ws, const std::pair< int, int >& offset)
  {
    std::stack< smcv::ExprElem > stack;
    stack.push(postfix.front());
    postfix.pop();

    while (!postfix.empty())
    {
      try
      {
        while (!isOperation(postfix.front()))
        {
          stack.push(postfix.front());
          postfix.pop();
        }
        char op = postfix.front().getOperation().getType();
        postfix.pop();
        double rhs = isOperand(stack.top()) ? stack.top().getOperand().getValue()
                                            : stack.top().getAddress().getValue(ws, offset);
        stack.pop();
        double lhs = isOperand(stack.top()) ? stack.top().getOperand().getValue()
                                            : stack.top().getAddress().getValue(ws, offset);
        stack.top() = smcv::ExprElem(smcv::ExprElem::Operand(calcOperation(op, lhs, rhs)));
      }
      catch (...)
      {
        throw std::logic_error("expression is incorrect");
      }
    }

    smcv::ExprElem ret = stack.top();
    stack.pop();
    if (!stack.empty())
    {
      throw std::logic_error("expression is incorrect");
    }

    return isOperand(ret) ? ret.getOperand().getValue() : ret.getAddress().getValue(ws, offset);
  }
}

semichev::Expression::Expression(double val): exprPostf_(), exprInf_()
{
  exprPostf_.push(ExprElem(ExprElem::Operand(val)));
  exprInf_.push(ExprElem(ExprElem::Operand(val)));
}

semichev::Expression::Expression(const expr_t& expr):
  exprPostf_(convertToPostfix(expr_t(expr))),
  exprInf_(expr_t(expr))
{}

double semichev::Expression::operator()(Spreadsheet& ws, const std::pair< int, int >& offset) const
{
  return calcPostfixExpr(expr_t(exprPostf_), ws, offset);
}

std::istream& semichev::Expression::read(std::istream& in)
{
  std::string str;
  smcv::Expression::expr_t queue;
  while (in >> str && str != "!")
  {
    try
    {
      queue.push(toExprElem(str));
    }
    catch (...)
    {
      in.setstate(in.failbit);
    }
  }

  if (in)
  {
    *this = Expression(queue);
  }

  return in;
}

std::ostream& semichev::Expression::print(std::ostream& out, const std::pair< int, int >& offset) const
{
  out << "=";
  auto q = exprInf_;
  while (!q.empty())
  {
    out << " ";
    switch (q.front().getType())
    {
    case ExprElem::Type::ADDRESS:
      out << q.front().getAddress().getCell().second + offset.second << ":";
      out << q.front().getAddress().getCell().first + offset.first;
      break;
    case ExprElem::Type::OPERAND:
      out << q.front().getOperand().getValue();
      break;
    case ExprElem::Type::OPERATION:
      out << q.front().getOperation().getType();
      break;
    case ExprElem::Type::PARENTHESIS:
      out << q.front().getParenthesis().getType();
      break;
    default:
      break;
    }
    q.pop();
  }
  return out << " !";
}
