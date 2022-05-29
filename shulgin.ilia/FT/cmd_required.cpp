#include "cmd_required.hpp"

#include <algorithm>
#include <sstream>

#include "stream_guard.hpp"

std::istream& shulgin::operator>>(std::istream& in, InputDelimiter&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

bool shulgin::isNumber(const std::string& str)
{
  return std::all_of(str.cbegin(), str.cend(), isdigit);
}

bool shulgin::checkStreamOn(std::istream& in, char symbol)
{
  shulgin::StreamGuard guard(in);
  in >> std::noskipws >> InputDelimiter{ symbol };
  return static_cast<bool>(in);
}

void shulgin::printMenu(std::ostream& out)
{
  out << "This cross reference program supports the following commands:\n";
  out << "read <text.txt> <dict>\n";
  out << "print <dict>\n";
  out << "printtext <text.txt>\n";
  out << "find <dict> <line>\n";
  out << "find <dict> <line> <text.txt>\n";
  out << "find <dict> <word>\n";
  out << "find <dict> <word> <text.txt>\n";
  out << "remove <dict>\n";
  out << "remove <dict> <line>\n";
  out << "remove <dict> <line> <text.txt>\n";
  out << "remove <dict> <word>\n";
  out << "remove <dict> <word> <text.txt>\n";
  out << "removetext <text.txt>\n";
  out << "unite <dict1> <dict2> <new_dict>\n";
  out << "cross <dict1> <dict2> <new_dict>\n";
  out << '\n';
}

std::ostream& shulgin::operator<<(std::ostream& out, const TextWordsDict& data)
{
  using ostreamIt = std::ostream_iterator< std::string >;
  std::transform(data.cbegin(), data.cend(), ostreamIt(out, "\n"), convertWordsAndTextToString);
  return out;
}

std::ostream& shulgin::operator<<(std::ostream& out, const TextLinesDict& data)
{
  using ostreamIt = std::ostream_iterator< std::string >;
  std::transform(data.cbegin(), data.cend(), ostreamIt(out, "\n"), convertLinesAndTextToString);
  return out;
}

std::ostream& shulgin::operator<<(std::ostream& out, const Words& words)
{
  using ostreamIt = std::ostream_iterator< std::string >;
  std::copy(words.cbegin(), words.cend(), ostreamIt(out, ", "));
  return out;
}

std::ostream& shulgin::operator<<(std::ostream& out, const Lines& lines)
{
  using ostreamIt = std::ostream_iterator< size_t >;
  std::copy(lines.cbegin(), lines.cend(), ostreamIt(out, " "));
  return out;
}

std::string shulgin::convertWordsAndTextToString(const TextAndWords& item)
{
  std::ostringstream out;
  out << item.first << ": ";
  using ostringstreamIter = std::ostream_iterator< std::string >;
  std::copy(item.second.cbegin(), item.second.cend(), ostringstreamIter(out, ", "));
  return out.str();
}

std::string shulgin::convertLinesAndTextToString(const TextAndLines& item)
{
  std::ostringstream out;
  out << item.first << ": ";
  using ostringstreamIter = std::ostream_iterator< size_t >;
  std::copy(item.second.cbegin(), item.second.cend(), ostringstreamIter(out, " "));
  return out.str();
}
