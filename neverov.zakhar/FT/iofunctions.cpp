#include "iofunctions.hpp"
#include <istream>
#include "comExceptions.hpp"

bool neverov::getNextWord(std::string& out, std::string& inString)
{
  inString = "";
  size_t nextSpace = out.find_first_of(' ');
  inString = out.substr(0, nextSpace);
  size_t charsForDelete = (nextSpace == std::string::npos) ? nextSpace : nextSpace + 1;
  out.erase(0, charsForDelete);
  return !inString.empty();
}

void neverov::inputText(std::istream& in, std::string& toText)
{
  std::string total{};
  while (in && !in.eof())
  {
    std::string line{};
    std::getline(in, line);
    total += line;
    if (!in.eof())
    {
      total += '\n';
    }
  }
  total.erase(total.size() - 1);
  toText = total;
  in.clear();
}

void neverov::outputHuffmanCode(std::ostream& out, const HTree& htree)
{
  for (auto&& p: htree.huffmanCode_)
  {
    out << p.first << ' ' << p.second << '\n';
  }
}

neverov::args_t neverov::getArgs(std::string& cmd, int requiredCount, int optionalCount)
{
  std::vector< std::string > args{};

  std::string curArg = "";
  bool isValid = true;
  for (int i = 0; i < requiredCount; ++i)
  {
    isValid = getNextWord(cmd, curArg) && isValid;
    args.push_back(curArg);
  }
  for (int i = 0; i < optionalCount; ++i)
  {
    if (getNextWord(cmd, curArg))
    {
      args.push_back(curArg);
    }
  }
  if (!cmd.empty() || !isValid)
  {
    throw invArgs;
  }
  return args;
}

std::pair< char, std::string > neverov::splitToCharStrPair(std::string line)
{
  std::vector< std::string > codes = getArgs(line, 2, 0);
  return {*codes[0].begin(), codes[1]};
}
