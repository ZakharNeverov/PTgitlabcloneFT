#ifndef GET_WORD_HPP
#define GET_WORD_HPP

#include <string>

namespace malakhov
{
  class GetWord
  {
  public:
    explicit GetWord(const std::string& source);
    ~GetWord() = default;

    bool ended();
    std::string get();
    void toNext();

    const std::string& ref;

  private:
    std::string::const_iterator wordBegin;
  };
}

#endif
