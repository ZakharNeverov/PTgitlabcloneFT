#include "Spreadsheet.hpp"

#include <algorithm>
#include <iterator>

namespace smcv = semichev;

semichev::Spreadsheet::Spreadsheet():
  buffer_(),
  offset_(),
  sheets_(),
  cur_(sheets_.begin())
{}

smcv::Spreadsheet::sheet_t& semichev::Spreadsheet::getCurrent()
{
  return cur_->second;
}

bool semichev::Spreadsheet::isEmpty()
{
  return cur_ == sheets_.end();
}

void semichev::Spreadsheet::setCurrent(const std::string& str)
{
  sheets_[str];
  cur_ = sheets_.find(str);
}

void semichev::Spreadsheet::deleteSheet(const std::string& str)
{
  if (cur_ == sheets_.find(str))
  {
    cur_ = sheets_.begin();
  }
  sheets_.erase(str);
}

const smcv::Spreadsheet::sheets_t& semichev::Spreadsheet::getSheets() const
{
  return sheets_;
}

void semichev::Spreadsheet::copy(const std::pair< int, int >& cell)
{
  offset_ = cell;
  buffer_.clear();
  buffer_[cell.first][cell.second] = cur_->second[cell.first][cell.second];
}

void semichev::Spreadsheet::copy(const std::pair< int, int >& from, const std::pair< int, int >& to)
{
  auto rows = std::minmax(from.first, to.first);
  auto cols = std::minmax(from.second, to.second);
  offset_ = { rows.first, cols.first };
  buffer_.clear();
  for (auto i = cur_->second.lower_bound(rows.first); i != cur_->second.upper_bound(rows.second); ++i)
  {
    auto it = std::inserter(buffer_[i->first], buffer_[i->first].end());
    std::copy(i->second.lower_bound(cols.first), i->second.upper_bound(cols.second), it);
  }
}

void semichev::Spreadsheet::paste(const std::pair< int, int >& cell)
{
  std::pair< int, int > diff = { cell.first - offset_.first, cell.second - offset_.second };
  for (auto i = buffer_.begin(); i != buffer_.end(); ++i)
  {
    for (auto j = i->second.begin(); j != i->second.end(); ++j)
    {
      auto& it = cur_->second[i->first + diff.first][j->first + diff.second] = j->second;
      it.addOffset(diff);
    }
  }
}

void semichev::Spreadsheet::paste(const std::pair< int, int >& from, const std::pair< int, int >& to)
{
  auto rows = std::minmax(from.first, to.first);
  auto cols = std::minmax(from.second, to.second);
  if (buffer_.size() == 1 && buffer_.begin()->second.size() == 1)
  {
    for (int i = rows.first; i <= rows.second; i++)
    {
      for (int j = cols.first; j <= cols.second; j++)
      {
        paste({ i, j });
      }
    }
  }
  else if (buffer_.size() >= 1 && buffer_.begin()->second.size() >= 1)
  {
    paste({ rows.first, cols.first });
  }
}
