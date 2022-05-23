#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <iosfwd>
#include <queue>
#include "ExprElem.hpp"
#include "Function.hpp"

namespace semichev
{

  class Expression: public Function
  {
   public:
    using expr_t = std::queue< ExprElem >;

    Expression() = delete;
    Expression(const Expression&) = default;
    Expression(Expression&&) = default;
    explicit Expression(double val);
    explicit Expression(const expr_t& expr);

    Expression& operator=(const Expression&) = default;
    Expression& operator=(Expression&&) = default;

    double operator()(Spreadsheet& ws, const std::pair<int, int>& offset) const;
    std::istream& read(std::istream& in);
    std::ostream& print(std::ostream& out, const std::pair<int, int>& offset) const;
   private:
    expr_t exprPostf_;
    expr_t exprInf_;
  };
}

#endif
