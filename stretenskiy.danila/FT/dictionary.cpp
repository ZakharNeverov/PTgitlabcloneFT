#include "dictionary.hpp"

#include <iostream>

namespace stretenskiy
{
  std::istream &operator>>(std::istream &in, Dictionary &dict)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    Dictionary temp;
    in >> DelimeterIO{'{'};
    in >> NameDictIO{temp.name_};
    while (!(in >> DelimeterIO{'}'}))
    {
      in.clear();
      std::string word;
      in >> WordIO{word} >> DelimeterIO{':'};
      if (!in)
      {
        doCleanStream(in);
        continue;
      }
      in >> TranslateIO{temp.content_[word]};
    }
    if (in)
    {
      dict = temp;
    }
    return in;
  }

  std::ostream &operator<<(std::ostream &out, const transl &transl)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    auto beg = transl.begin();
    auto end = transl.end();
    end--;
    for (auto i = beg; i != end; ++i)
    {
      out << '"' << *i << "\", ";
    }
    out << '"' << *end << "\"|";
    return out;
  }

  std::ostream &operator<<(std::ostream &out, const content &content)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }

    auto beg = content.begin();
    auto end = content.end();
    for (auto i = beg; i != end; ++i)
    {
      auto it = i;
      it++;
      out << '"' << i->first << "\": ";
      if (it == end)
      {
        out << i->second;
        break;
      }
      out << i->second << ";\n";
    }
    return out;
  }

  std::ostream &operator<<(std::ostream &out, const Dictionary &dict)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    out << "{DICTIONARY " << '"' << dict.name_ << "\"|\n";
    out << dict.content_ << '}';
    return out;
  }

  std::ostream &commandPrintDict(std::ostream &out, const Dictionary &dict)
  {
    out << "DICTIONARY " << dict.name_ << '\n';
    for (const auto &i: dict.content_)
    {
      out << i.first << ':';
      for (const auto &j: i.second)
      {
        out << ' ' << j;
      }
      out << '\n';
    }
    return out;
  }
}
