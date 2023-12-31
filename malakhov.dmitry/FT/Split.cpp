#include "Split.hpp"

#include "GetWord.hpp"

void malakhov::split(const std::string& input, std::string& commandName, Commands::ArgsListT& args)
{
  GetWord getWord{input};
  commandName = getWord.get();
  getWord.toNext();
  while (!getWord.ended())
  {
    args.push_back(getWord.get());
    getWord.toNext();
  }
}
