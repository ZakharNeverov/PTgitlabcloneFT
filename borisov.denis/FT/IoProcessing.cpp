#include "IoProcessing.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include "DelimiterIO.h"
#include "IoFmtGuard.h"

namespace
{
  class WordPrinter
  {
  public:
    WordPrinter();
    WordPrinter(const WordPrinter&) = default;
    ~WordPrinter() = default;
    std::ostream& operator()(std::ostream& out, const std::pair< borisov::Word, borisov::TranslationList >& p);
  private:
    bool isFirst_;
  };

  std::ostream& printWordTranslationCount(std::ostream& out, const std::string& word, const borisov::TranslationPair& p);
  std::ostream& printSpaceWordTranslationCount(std::ostream& out, const std::string& word, const borisov::TranslationPair& p);
}

std::istream& borisov::restoreIStream(std::istream& in)
{
  in.clear();
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  return in;
}

std::istream& borisov::getDict(std::istream& in, borisov::Dict& dict)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  IoFmtGuard guard(in);
  in >> std::noskipws;
  while (in)
  {
    std::string word;
    std::string translation;
    std::string countStr;
    in >> word >> DelimiterIO{' '} >> translation >> DelimiterIO{' '} >> countStr;
    size_t count = std::stoull(countStr);
    if (in)
    {
      borisov::add(dict, word, translation, count);
    }
    char ch = in.peek();
    if (ch == '\n')
    {
      break;
    }
    else
    {
      in >> DelimiterIO{' '};
    }
  }
  return in;
}

std::ostream& borisov::printDict(std::ostream& out, const borisov::Dict& dict)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  using namespace std::placeholders;
  std::for_each(
    dict.cbegin(), dict.cend(),
    std::bind(WordPrinter{}, std::ref(out), _1)
  );
  return out;
}

std::istream& borisov::getArgList(std::istream& in, borisov::ArgList& argList)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string s;
  IoFmtGuard guard(in);
  in >> std::noskipws;
  while (in)
  {
    in >> s;
    if (in)
    {
      argList.push_back(s);
    }
    char ch = in.peek();
    if (ch == '\n')
    {
      in >> DelimiterIO{'\n'};
      break;
    }
    else
    {
      in >> DelimiterIO{' '};
    }
  }
  return in;
}

namespace
{
  WordPrinter::WordPrinter():
    isFirst_(true)
  {
  }

  std::ostream& WordPrinter::operator()(std::ostream& out, const std::pair< borisov::Word, borisov::TranslationList >& p)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    auto translationIter = p.second.cbegin();
    auto translationIterEnd = p.second.cend();
    if (isFirst_)
    {
      if (translationIter != translationIterEnd)
      {
        printWordTranslationCount(out, p.first, *(translationIter++));
      }
      isFirst_ = false;
    }
    using namespace std::placeholders;
    std::for_each(
      translationIter, translationIterEnd,
      std::bind(printSpaceWordTranslationCount, std::ref(out), std::ref(p.first), _1)
    );
    return out;
  }

  std::ostream& printWordTranslationCount(std::ostream& out, const std::string& word, const borisov::TranslationPair& p)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    out << word << ' ' << p.first << ' ' << p.second;
    return out;
  }

  std::ostream& printSpaceWordTranslationCount(std::ostream& out, const std::string& word, const borisov::TranslationPair& p)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    out << ' ';
    printWordTranslationCount(out, word, p);
    return out;
  }
}
