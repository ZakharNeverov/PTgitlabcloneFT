#include "dictionary.hpp"

namespace krylyanok
{
  struct Commands
  {
  public:
    Commands(std::vector< NameMap >& dictionaries, std::istream& in, std::ostream& out):
      dictionaries_(dictionaries),
      in_(in),
      out_(out)
    {}
    void getLoad();
    void getPrint();
    void getTop();
    void getSearch();
    void getMerge();
    void getDelete();
    void getSave();
    void getDeleteWord();
    void getInsertWord();

  private:
    std::vector< NameMap >& dictionaries_;
    std::istream& in_;
    std::ostream& out_;
  };
}
