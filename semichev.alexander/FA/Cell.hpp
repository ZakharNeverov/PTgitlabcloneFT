#ifndef CELL_HPP
#define CELL_HPP

#include "Expression.hpp"
#include <memory>
#include <utility>

namespace semichev
{
  class Cell
  {
   public:
    Cell();
    Cell(const Cell& other) = default;
    Cell(Cell&& other) = default;
    Cell(const std::shared_ptr<Function>& func);

    Cell& operator=(const Cell& other) = default;
    Cell& operator=(Cell&& other) = default;

    std::pair<std::pair<int, int>, Function&> getFunction() const;
    double getValue(Spreadsheet& ws);
    void addOffset(const std::pair< int, int >& offset);

   private:
    bool isLoop_;
    std::pair< int, int > offset_;
    std::shared_ptr<Function> func_;
  };
}

#endif
