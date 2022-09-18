#include "Commands.hpp"

#include <iostream>
#include <fstream>

#include "Prints.hpp"

namespace
{
  using malakhov::Commands;
  using StrsDictT = Commands::StrsDictT;
  using ArgsListT = Commands::ArgsListT;

  void print(const StrsDictT& strs, const ArgsListT& args, std::ostream& out)
  {
    if (args.getSize() != 1)
    {
      throw std::logic_error("Not enough args in print");
    }
    const malakhov::HaffmanStringUnion& data = strs[*(args.getBegin())];
    if (!data.isEncoded() && data.getDecoded().empty())
    {
      malakhov::printEmpty(out) << '\n';
      return;
    }
    out << data << '\n';
  }

  void equals(const StrsDictT& strs, const ArgsListT& args, std::ostream& out)
  {
    if (args.getSize() != 2)
    {
      throw std::logic_error("Not enough args in equals");
    }
    auto it = args.getBegin();
    const malakhov::HaffmanStringUnion& a = strs[*it];
    ++it;
    const malakhov::HaffmanStringUnion& b = strs[*it];
    (a == b ? malakhov::printTrue(out) : malakhov::printFalse(out)) << '\n';
  }

  void flush(const StrsDictT& strs, const ArgsListT& args, std::ostream& out)
  {
    if (args.getSize() < 1 || args.getSize() > 2)
    {
      throw std::logic_error("Not enought args in flush");
    }
    auto it = args.getBegin();
    const std::string strName = *it;
    std::ofstream fout;
    {
      ++it;
      const std::string filename = it == args.getEnd() ? std::string{} : *it;
      if (!filename.empty())
      {
        fout.open(filename);
        if (!fout.is_open())
        {
          throw std::logic_error("Out file was not opened");
        }
      }
    }
    std::ostream& trueOut = fout.is_open() ? fout : out;
    trueOut << strs[strName] << '\n';
    if (!trueOut)
    {
      throw std::out_of_range("Something happened with out stream");
    }
  }

  void inspect(const StrsDictT& strs, const ArgsListT& args, std::ostream& out)
  {
    if (args.getSize() != 1)
    {
      throw std::logic_error("Not enough args in inspect");
    }
    const malakhov::HaffmanStringUnion& data = strs[*(args.getBegin())];
    (data.isEncoded() ? malakhov::printEncoded(out) : malakhov::printRaw(out)) << '\n';
  }

  void list(const StrsDictT& strs, const ArgsListT& args, std::ostream& out)
  {
    if (args.getSize())
    {
      throw std::logic_error("Not enough args in list");
    }
    auto it = strs.getBegin();
    if (it != strs.getEnd())
    {
      for (; it != strs.getAlmostEnd(); ++it)
      {
        out << it->first << ' ';
      }
      out << it->first;
    }
    else
    {
      malakhov::printEmpty(out);
    }
    out << '\n';
  }

  void drop(StrsDictT& strs, const ArgsListT& args)
  {
    if (args.getSize() != 1)
    {
      throw std::logic_error("Not enough args in drop");
    }
    for (auto it = args.getBegin(); it != args.getEnd(); ++it)
    {
      strs.pop(*it); //попыт ыыыыы
    }
  }

  void encode(StrsDictT& strs, const ArgsListT& args)
  {
    if (args.getSize() != 2)
    {
      throw std::logic_error("Not enough args in encode");
    }
    auto it = args.getBegin();
    const std::string& varName = *it;
    ++it;
    const std::string& decoded = strs[*it].getDecoded();
    strs.insert(varName, malakhov::HaffmanStringUnion{malakhov::Haffman::produce(decoded)});
  }

  void decode(StrsDictT& strs, const ArgsListT& args)
  {
    if (args.getSize() != 2)
    {
      throw std::logic_error("Not enough args in decode");
    }
    auto it = args.getBegin();
    const std::string& varName = *it;
    ++it;
    const malakhov::Haffman& encoded = strs[*it].getEncoded();
    strs.insert(varName, malakhov::HaffmanStringUnion{encoded.decode()});
  }

  void merge(StrsDictT& strs, const ArgsListT& args)
  {
    if (args.getSize() != 3)
    {
      throw std::logic_error("Not enough args in merge");
    }
    auto it = args.getBegin();
    const std::string& newVarName = *it;
    ++it;
    const std::string& aName = *it;
    auto& a = strs[aName];
    ++it;
    const std::string& bName = *it;
    auto b = strs[bName];
    if (a.isEncoded() != b.isEncoded())
    {
      throw std::logic_error("Merging encoded with decoded is forbidden");
    }
    if (a == b)
    {
      strs.pop(aName);
      strs.pop(bName);
      strs.insert(newVarName, b);
    }
  }

  void read(StrsDictT& strs, const ArgsListT& args)
  {
    if (args.getSize() < 1 || args.getSize() > 2)
    {
      throw std::logic_error("Not enought args in read");
    }
    auto it = args.getBegin();
    const std::string strName = *it;
    std::ifstream fin;
    {
      ++it;
      const std::string filename = it == args.getEnd() ? std::string{} : *it;
      if (!filename.empty())
      {
        fin.open(filename);
        if (!fin.is_open())
        {
          throw std::logic_error("Out file was not opened");
        }
      }
    }
    std::istream& trueIn = fin.is_open() ? fin : std::cin;
    malakhov::HaffmanStringUnion un;
    trueIn >> un;
    if (!trueIn)
    {
      throw std::out_of_range("Something happened with in stream");
    }
    strs.insert(strName, un);
  }

  void concat(StrsDictT& strs, const ArgsListT& args)
  {
    if (args.getSize() < 3)
    {
      throw std::logic_error("Not enough args in concat");
    }

    auto it = args.getBegin();
    const std::string& newVarName = *it;
    malakhov::HaffmanStringUnion toInsert;
    ++it;
    const malakhov::HaffmanStringUnion& first = strs[*it];
    ++it;
    const malakhov::HaffmanStringUnion& second = strs[*it];

    if (first.isEncoded() != second.isEncoded())
    {
      throw std::logic_error("Trying to concat encoded and decoded string is not allowed");
    }

    if (first.isEncoded())
    {
      const std::string decodedFirst = first.getEncoded().decode();
      const std::string decodedSecond = second.getEncoded().decode();
      const std::string decodedResult = decodedFirst + decodedSecond;
      const malakhov::Haffman result = malakhov::Haffman::produce(decodedResult);
      toInsert.set(result);
    }
    else
    {
      toInsert.set(first.getDecoded() + second.getDecoded());
    }
    strs.insert(newVarName, toInsert);
  }
}

malakhov::Commands::Commands(StrsDictT& strs, std::ostream& out):
  strs_(strs),
  commands_({{"drop", drop}, {"encode", encode}, {"decode", decode}, {"merge", merge}, {"concat", concat}, {"read", read}}),
  constCommands_({{"inspect", inspect}, {"print", print}, {"flush", flush}, {"equals", equals}, {"list", list}}),
  out_(out)
{}

void malakhov::Commands::call(const StrT& commandName, const ArgsListT& args) const noexcept
{
  try
  {
    auto command = constCommands_.find(commandName);
    if (command != constCommands_.getEnd())
    {
      (*(*command).second)(strs_, args, out_);
    }
    else
    {
      auto command = commands_.find(commandName);
      if (command != commands_.getEnd())
      {
        (*(*command).second)(strs_, args);
      }
      else
      {
        throw std::logic_error("No such command");
      }
    }
  }
  catch (const std::logic_error& e)
  {
    printInvalid(out_) << '\n';
  }
}
