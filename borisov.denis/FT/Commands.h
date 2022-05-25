#ifndef COMMANDS_H
#define COMMANDS_H
#include <iosfwd>
#include <map>
#include <string>

namespace borisov
{
  using Word = std::string;
  using Translation = std::string;
  using TranslationPair = std::pair< Translation, size_t >;
  using TranslationList = std::map< Translation, size_t >;
  using Dict = std::map< Word, TranslationList >;

  void add(Dict& dict, const Word& word, const Translation& translation, size_t count);
  void remove(Dict& dict, const Word& word);
  void remove(Dict& dict, const Word& word, const Translation& translation);
  void manual(Dict& dict, const Word& word, const Translation& translation , long long count);
  std::ostream& words(std::ostream& out, const Dict& dict, char sep = '\n');
  std::ostream& info(std::ostream& out, const Dict& dict, const Word& word, char freqSep = '\t', char sep = '\n');
  std::ostream& translate(std::ostream& out, const Dict& dict, const Word& word);
  std::ostream& save(std::ostream& out, const std::string& name, const Dict& dict);

  void unite(Dict& result, const Dict& first, const Dict& second);
  void intersect(Dict& result, const Dict& first, const Dict& second);
  void complement(Dict& result, const Dict& first, const Dict& second);
}
#endif
