#include "commands.hpp"
#include "io_structs.hpp"
#include "StreamGuard.hpp"
#include "CommandsMessages.hpp"

namespace
{
  bool checkEndArgs(kurzov::istream_t& in)
  {
    kurzov::StreamGuard guard(in);
    in >> std::noskipws >> kurzov::DelimeterIO{'\n'};
    return static_cast< bool >(in);
  }
  bool isValidDict(kurzov::dicts_t::iterator begin, kurzov::dicts_t& dicts)
  {
    return begin != dicts.end();
  }
}

bool kurzov::doPrint(istream_t& in, dicts_t& dicts, std::ostream& out)
{
  std::string dict_name = "";
  in >> dict_name;
  dicts_t::iterator dict_to_print_iter = dicts.find(dict_name);

  if (!isValidDict(dict_to_print_iter, dicts) || !checkEndArgs(in))
  {
    return false;
  }

  const dict_t& dict_to_print = dict_to_print_iter->second;
  if (dict_to_print.empty())
  {
    kurzov::outEmpty(out);
  }
  else
  {
    kurzov::printDict(dict_to_print, out, ",", false);
  }

  return true;
}

bool kurzov::doUnion(istream_t&, dicts_t&)
{
  return 1;
}

bool kurzov::doComplement(istream_t&, dicts_t&)
{
  return 1;
}

bool kurzov::doIntersect(istream_t&, dicts_t&)
{
  return 1;
}

bool kurzov::doLoad(istream_t&, dicts_t&)
{
  return 1;
}

bool kurzov::doTranslate(istream_t&, dicts_t&, std::ostream&)
{
  return 1;
}
