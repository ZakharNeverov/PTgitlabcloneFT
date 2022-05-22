#ifndef TABLE_HPP
#define TABLE_HPP

#include <string>
#include <map>
#include <iosfwd>
#include <vector>

namespace abraamyan
{
  class Table
  {
  public:
    Table(std::string name);
    Table(const Table &table);
    Table(Table &&table);
    bool insert(const std::string key, const size_t lineNum);
    bool find(const std::string key) const;
    size_t count(const std::string key) const;
    const std::string &getName() const;
    void setName(std::string name);
    std::ostream &printTable(std::ostream &out) const;
    Table unite(const Table &table1, std::string name) const;
    Table intersect(const Table &table1, std::string name) const;

  private:
    std::string name_;
    std::map < std::string, std::vector< size_t > > map_;
  };
}
#endif
