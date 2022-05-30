#ifndef IOPROCCESSING_HPP
#define IOPROCCESSING_HPP

#include <ios>
#include <set>

namespace stretenskiy
{
  class IoFmtGuard
  {
  public:
    IoFmtGuard(std::basic_ios< char > &s);
    ~IoFmtGuard();
  private:
    std::basic_ios< char > &s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  struct DelimeterIO
  {
    char exp;
  };

  struct DelimeterNameIO
  {
    std::string exp;
  };

  struct WordIO
  {
    std::string &ref;
  };

  struct NameDictIO
  {
    std::string &ref;
  };

  struct TranslateIO
  {
    std::set < std::string > &ref;
  };

  bool checkContinueInputWord(std::istream &);
  void doCleanStream(std::istream &);
  std::istream &operator>>(std::istream &, DelimeterIO &&);
  std::istream &operator>>(std::istream &, DelimeterNameIO &&);
  std::istream &operator>>(std::istream &, WordIO &&);
  std::istream &operator>>(std::istream &, NameDictIO &&);
  std::istream &operator>>(std::istream &, TranslateIO &&);
}

#endif
