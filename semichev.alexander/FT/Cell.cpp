#include "Cell.hpp"

#include <stdexcept>

namespace smcv = semichev;

semichev::Cell::Cell():
  isLoop_(false),
  offset_(0, 0),
  func_(std::make_shared< Expression >(0))
{}

semichev::Cell::Cell(const std::shared_ptr< Function >& func):
  isLoop_(false),
  offset_(0, 0),
  func_(func)
{}

std::pair< std::pair< int, int >, smcv::Function& > semichev::Cell::getFunction() const
{
  return { offset_, *func_ };
}

double semichev::Cell::getValue(Spreadsheet& ws)
{
  if (isLoop_)
  {
    throw std::logic_error("expr is loop");
  }
  isLoop_ = true;
  double ret = func_->operator()(ws, offset_);
  isLoop_ = false;
  return ret;
}

void semichev::Cell::addOffset(const std::pair< int, int >& offset)
{
  offset_ = { offset_.first + offset.first, offset_.second + offset.second };
}
