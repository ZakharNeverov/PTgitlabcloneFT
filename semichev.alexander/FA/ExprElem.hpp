#ifndef EXPRESSION_ELEMENT_HPP
#define EXPRESSION_ELEMENT_HPP

#include <iosfwd>
#include <string>

#include "ForwardDeclaration.hpp"

namespace semichev
{
  class ExprElem
  {
   public:
    enum class Type
    {
      OPERAND,
      ADDRESS,
      OPERATION,
      PARENTHESIS
    };

    class Operand
    {
     public:
      Operand() = delete;
      Operand(const Operand& other) = default;
      Operand(Operand&& other) noexcept = default;
      explicit Operand(double val);

      Operand& operator=(const Operand& other) = default;
      Operand& operator=(Operand&& other) noexcept = default;

      double getValue() const;

     private:
      double val_;
    };

    class Address
    {
    public:
        Address() = delete;
        Address(const Address& other) = default;
        Address(Address&& other) noexcept = default;
        explicit Address(int column, int row);

        Address& operator=(const Address& other) = default;
        Address& operator=(Address&& other) noexcept = default;

        double getValue(Spreadsheet& ws, const std::pair<int, int>& offset) const;
        std::pair<int, int> getCell() const;

    private:
        int column_;
        int row_;
    };

    class Operation
    {
     public:
      Operation() = delete;
      Operation(const Operation& other) = default;
      Operation(Operation&& other) noexcept = default;

      Operation& operator=(const Operation& other) = default;
      Operation& operator=(Operation&& other) noexcept = default;

      static Operation getAdd();
      static Operation getSub();
      static Operation getMul();
      static Operation getDiv();

      char getType() const;
      bool hasMorePriority(const Operation& other) const;

     private:
      Operation(char type, int priority);

      char type_;
      int priority_;
    };

    class Parenthesis
    {
     public:
      Parenthesis() = delete;
      Parenthesis(const Parenthesis& other) = default;
      Parenthesis(Parenthesis&& other) noexcept = default;

      Parenthesis& operator=(const Parenthesis& other) = default;
      Parenthesis& operator=(Parenthesis&& other) noexcept = default;

      static Parenthesis getLeft();
      static Parenthesis getRight();

      char getType() const;

     private:
      explicit Parenthesis(char type);

      char type_;
    };

    ExprElem() = delete;
    ExprElem(const ExprElem& other) = default;
    ExprElem(ExprElem&& other) noexcept = default;
    explicit ExprElem(const Operand& val);
    explicit ExprElem(const Address& val);
    explicit ExprElem(const Operation& val);
    explicit ExprElem(const Parenthesis& val);

    ExprElem& operator=(const ExprElem& other) = default;
    ExprElem& operator=(ExprElem&& other) noexcept = default;

    Type getType() const;
    Operand getOperand() const;
    Address getAddress() const;
    Operation getOperation() const;
    Parenthesis getParenthesis() const;

   private:
    Type type_;
    union
    {
      Operand operand_;
      Address address_;
      Operation operation_;
      Parenthesis parenthesis_;
    };
  };

  ExprElem toExprElem(const std::string& str);
}

#endif
