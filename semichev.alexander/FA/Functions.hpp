#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <string>
#include "Function.hpp"
#include <memory>

namespace semichev
{
    class RangeFunction : public Function
    {
    public:
        RangeFunction() = delete;
        RangeFunction(const RangeFunction& other) = default;
        RangeFunction(RangeFunction&& other) = default;
        RangeFunction(const std::pair<int, int>&from, const std::pair<int, int>&to);

        RangeFunction& operator=(const RangeFunction& other) = default;
        RangeFunction& operator=(RangeFunction&& other) = default;

     protected:
        std::pair<int, int> rows_;
        std::pair<int, int> cols_;
    };

    class Sum : public RangeFunction
    {
    public:
        Sum() = delete;
        Sum(const Sum& other) = default;
        Sum(Sum&& other) = default;
        Sum(const std::pair<int, int>& from, const std::pair<int, int>& to);

        Sum& operator=(const Sum& other) = default;
        Sum& operator=(Sum&& other) = default;

        double operator()(Spreadsheet& ws, const std::pair<int, int>& offset) const;
        std::istream& read(std::istream& in);
        std::ostream& print(std::ostream& out, const std::pair<int, int>& offset) const;
    };

  class Average: public RangeFunction
  {
   public:
    Average() = delete;
    Average(const Average& other) = default;
    Average(Average&& other) = default;
    Average(const std::pair<int, int>& from, const std::pair<int, int>& to);

    Average& operator=(const Average& other) = default;
    Average& operator=(Average&& other) = default;
   
    double operator()(Spreadsheet& ws, const std::pair<int, int>& offset) const;
      std::istream& read(std::istream& in);
      std::ostream& print(std::ostream& out, const std::pair<int, int>& offset) const;
  };

  class Count: public RangeFunction
  {
   public:
    Count() = delete;
    Count(const Count& other) = default;
    Count(Count&& other) = default;
    Count(const std::pair<int, int>& from, const std::pair<int, int>& to, const std::shared_ptr<Function>& func);

    Count& operator=(const Count& other) = default;
    Count& operator=(Count&& other) = default;
   
    double operator()(Spreadsheet& ws, const std::pair<int, int>& offset) const;
    std::istream& read(std::istream& in);
    std::ostream& print(std::ostream& out, const std::pair<int, int>& offset) const;
   private:
       std::shared_ptr<Function> func_;
       
  };

  class Min : public RangeFunction
  {
  public:
      Min() = delete;
      Min(const Min& other) = default;
      Min(Min&& other) = default;
      Min(const std::pair<int, int>& from, const std::pair<int, int>& to);

      Min& operator=(const Min& other) = default;
      Min& operator=(Min&& other) = default;

      double operator()(Spreadsheet& ws, const std::pair<int, int>& offset) const;
      std::istream& read(std::istream& in);
      std::ostream& print(std::ostream& out, const std::pair<int, int>& offset) const;
  };

  class Max : public RangeFunction
  {
  public:
      Max() = default;
      Max(const Max& other) = default;
      Max(Max&& other) noexcept = default;
      Max(const std::pair<int, int>& from, const std::pair<int, int>& to);

      Max& operator=(const Max& other) = default;
      Max& operator=(Max&& other) = default;

      double operator()(Spreadsheet& ws, const std::pair<int, int>& offset) const;
      std::istream& read(std::istream& in);
      std::ostream& print(std::ostream& out, const std::pair<int, int>& offset) const;
  };
}

#endif

