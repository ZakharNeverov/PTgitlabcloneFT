#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <stdexcept>
#include <algorithm>
#include <functional>

#include "cross_references.hpp"
#include "cmd_required.hpp"

namespace shulgin
{
  using RefDicts = std::vector< CrossReferences >;

  bool isName(const CrossReferences&, const std::string&);

  namespace detail
  {
    template< class T >
    auto tryFindInRefDicts(T& data, const std::string& name)
    {
      using namespace std::placeholders;
      auto it = std::find_if(data.begin(), data.end(), std::bind(isName, _1, name));
      if (it == data.end())
      {
        throw std::logic_error("Error, can not find the <" + name + ">!");
      }
      return it;
    }
  }

  void doReadCmd(RefDicts&, std::istream&);

  void doPrintCmd(const RefDicts&, std::istream&, std::ostream&);

  void doPrintTextCmd(std::istream&, std::ostream&);

  void doFindCmd(const RefDicts&, std::istream&, std::ostream&);
  TextWordsDict findCertainWords(const CrossReferences&, size_t);
  TextWordsDict& accumulateWordsWithLine(TextWordsDict&, const WordAndTexts&, size_t);
  TextLinesDict findCertainLines(const CrossReferences&, const std::string&);
  Words findCertainWordsInText(const CrossReferences&, size_t, const std::string&);
  Words& accumulateWordsWithLineInText(Words&, const WordAndTexts&, size_t, const std::string&);
  Lines findCertainLinesInText(const CrossReferences&, const std::string&, const std::string&);

  void doRemoveCmd(RefDicts&, std::istream&, std::ostream&);
  bool removeLine(CrossReferences&, size_t, const std::string& textName = "");
  bool removeWord(CrossReferences&, const std::string&, const std::string& textName = "");

  void doRemoveTextCmd(RefDicts&, std::istream&, std::ostream&);
  void removeText(CrossReferences&, const std::string&);

  void doUniteCmd(RefDicts&, std::istream&, std::ostream&);
  CrossReferencesDict uniteRefDicts(const CrossReferencesDict&, const CrossReferencesDict&);
  TextLinesListDict uniteTextLinesDicts(const TextLinesListDict&, const TextLinesListDict&);
  LinesList uniteLinesLists(const LinesList&, const LinesList&);
  Texts uniteTexts(const Texts&, const Texts&);

  void doCrossCmd(RefDicts&, std::istream&, std::ostream&);
  CrossReferencesDict crossRefDicts(const CrossReferencesDict&, const CrossReferencesDict&, Texts&);
}

#endif
