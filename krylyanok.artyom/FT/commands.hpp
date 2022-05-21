#include "hash-dictionary.hpp"

namespace krylyanok
{
  struct Commands
  {
  public:
    Commands(std::vector< krylyanok::Dictionary >& dictionaries, std::istream& in, std::ostream& out):
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
  private:
    std::vector< krylyanok::Dictionary >& dictionaries_;
    std::istream& in_;
    std::ostream& out_;
    void streamClean();
    size_t findElem(std::string& dictName);
  };
}
