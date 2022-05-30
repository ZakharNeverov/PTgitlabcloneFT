#include "table.hpp"

#include <iomanip>
#include <algorithm>


namespace abraamyan
{
  Table::Table(std::string name):
    name_(name)
  { }

  Table::Table(const Table &table):
    name_(table.name_),
    map_(table.map_)
  { }

  Table::Table(Table &&table):
    name_(std::move(table.name_)),
    map_(std::move(table.map_))
  { }

  Table &Table::operator=(const Table &table)
  {
    name_ = table.name_;
    map_ = table.map_;
    return *this;
  }

  bool Table::operator==(const Table &table) const
  {
    return (name_ == table.name_) && (map_ == table.map_);
  }

  bool Table::insert(const std::string key, const size_t lineNum)
  {
    auto it = map_.find(key);
    if (it == map_.end())
    {
      map_[key].push_back(lineNum);
      return true;
    }
    if (std::find(it->second.begin(), it->second.end(), lineNum) != it->second.end())
    {
      return false;
    }
    else
    {
      it->second.push_back(lineNum);
      std::sort(map_[key].begin(), map_[key].end());
      return true;
    }
  }

  bool Table::find(const std::string key) const
  {
    return map_.find(key) != map_.end();
  }

  size_t Table::count(const std::string key) const
  {
    auto it = map_.find(key);
    if (it != map_.end())
    {
      return it->second.size();
    }
    return 0ul;
  }

  const std::string &Table::getName() const
  {
    return name_;
  }

  void Table::setName(std::string name)
  {
    name_ = name;
  }

  std::ostream &Table::printTable(std::ostream &out) const
  {
    for (auto &it : map_)
    {
      out << std::setw(30) << it.first << ": ";
      for (size_t i = 0ul; i < it.second.size() - 1; i++)
      {
        out << it.second[i] << ", ";
      }
      out << it.second[it.second.size() - 1] << '\n';
    }
    return out;
  }

  Table Table::unite(const Table &table1, std::string name) const
  {
    auto it = table1.map_.cbegin();
    auto end = table1.map_.cend();
    Table newTable(*this);
    newTable.name_ = std::move(name);
    while (it != end)
    {
      for (size_t i = 0; i < it->second.size(); i++)
      {
        newTable.insert(it->first, it->second[i]);
      }
      it++;
    }
    return newTable;
  }

  Table Table::intersect(const Table &table1, std::string name) const
  {
    auto it = map_.cbegin();
    auto end = map_.cend();
    auto it1 = table1.map_.cbegin();
    auto end1 = table1.map_.cend();
    Table newTable(std::move(name));
    while (it != end && it1 != end1)
    {
      if (it->first == it1->first)
      {
        size_t i = 0;
        size_t k = 0;
        while (i < it->second.size() && k < it1->second.size())
        {
          if (it->second[i] == it1->second[k])
          {
            newTable.insert(it->first, it->second[i]);
            i++;
            k++;
          }
          else if (it->second[i] > it1->second[k])
          {
            k++;
          }
          else
          {
            i++;
          }
        }
        it++;
        it1++;
      }
      if (it->first > it1->first)
      {
        it1++;
      }
      else
      {
        it++;
      }
    }
    return newTable;
  }
}
