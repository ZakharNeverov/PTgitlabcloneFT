#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include "dictionaryReading.hpp"

namespace bazhenov {
  using dictionaries = std::map< std::string, freq >;

  struct print_t {
    print_t() = delete;
    print_t(std::istream& in, std::ostream& out);
    void operator()(dictionaries& storage);
    private:
    std::istream& in_;
    std::ostream& out_;
  };

  struct count_t {
    count_t() = delete;
    count_t(std::istream& in, std::ostream& out);
    void operator()(dictionaries& storage);
    private:
    std::istream& in_;
    std::ostream& out_;
  };

  struct clear_t {
    clear_t() = delete;
    clear_t(std::istream& in);
    void operator()(dictionaries& storage);
    private:
    std::istream& in_;
  };

  struct find_t {
    find_t() = delete;
    find_t(std::istream& in, std::ostream& out);
    void operator()(dictionaries& storage);
    private:
    std::istream& in_;
    std::ostream& out_;
  };

  struct upper_t {
    upper_t() = delete;
    upper_t(std::istream& in, std::ostream& out, std::string extreme_);
    void operator()(dictionaries& storage);
    private:
    std::istream& in_;
    std::ostream& out_;
    std::string extreme_;
  };

  struct search_pref_t {
    search_pref_t() = delete;
    search_pref_t(std::istream& in, std::ostream& out);
    void operator()(dictionaries& storage);
    private:
    std::istream& in_;
    std::ostream& out_;
  };

  struct search_t {
    search_t() = delete;
    search_t(std::istream& in, std::ostream& out);
    void operator()(dictionaries& storage);
    private:
    std::istream& in_;
    std::ostream& out_;
  };

  struct add_t {
    add_t() = delete;
    add_t(std::istream& in, std::ostream& out);
    void operator()(dictionaries& storage);
    private:
    std::istream& in_;
    std::ostream& out_;
  };

  struct diff_t {
    diff_t() = delete;
    diff_t(std::istream& in, std::ostream& out);
    void operator()(dictionaries& storage);
    private:
    std::istream& in_;
    std::ostream& out_;
  };
}

#endif
