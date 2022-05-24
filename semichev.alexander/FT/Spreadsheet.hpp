#ifndef SPREADSHEET_HPP
#define SPREADSHEET_HPP

#include <map>
#include <string>

#include "Cell.hpp"

namespace semichev
{
  class Spreadsheet
  {
   public:
    using sheet_t = std::map< int, std::map< int, Cell > >;
    using sheets_t = std::map< std::string, sheet_t >;
    Spreadsheet();
    Spreadsheet(const Spreadsheet& other) = default;
    Spreadsheet(Spreadsheet&& other) = default;

    Spreadsheet& operator=(const Spreadsheet& other) = default;
    Spreadsheet& operator=(Spreadsheet&& other) = default;

    bool isEmpty();

    sheet_t& getCurrent();
    void setCurrent(const std::string& str);
    void deleteSheet(const std::string& str);
    const sheets_t& getSheets() const;

    void copy(const std::pair< int, int >& cell);
    void copy(const std::pair< int, int >& from, const std::pair< int, int >& to);

    void paste(const std::pair< int, int >& cell);
    void paste(const std::pair< int, int >& from, const std::pair< int, int >& to);

   private:
    sheet_t buffer_;
    std::pair< int, int > offset_;
    sheets_t sheets_;
    sheets_t::iterator cur_;
  };

  std::istream& operator>>(std::ostream& out, Spreadsheet& ws);
  std::ostream& operator<<(std::ostream& out, const Spreadsheet& ws);
}

#endif
