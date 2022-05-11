#ifndef TEXT_ANALYZER_H
#define TEXT_ANALYZER_H

#include <ios>
#include <string>
#include <map>
#include <vector>

namespace teslenko
{
  class TextAnalyzer
  {
  public:
    TextAnalyzer();
    TextAnalyzer(const TextAnalyzer& other) = delete;
    TextAnalyzer(TextAnalyzer&& other) noexcept;
    ~TextAnalyzer() = default;
    TextAnalyzer& operator=(const TextAnalyzer& other) = delete;
    TextAnalyzer& operator=(TextAnalyzer&& other) noexcept;

    const std::map< std::string, std::vector< int > >& getDictionary() const;
    void printDictionary(std::map< std::string, std::vector< int > >& dictionary_, std::ostream& os);

    std::map< std::string, std::vector< int > > analyze(const std::string& filename);
    void printAnalysis(std::ostream& os, const std::string& filename);
    void uniteDictionaries(std::vector< std::map< std::string, std::vector< int > > >& arrayDict, std::ostream& os);
    void subtractionDictionaries(std::vector< std::map< std::string, std::vector< int > > >& arrayDict, std::ostream& os);
    void getIntersectionDictionaries(std::vector< std::map< std::string, std::vector< int > > >& arrayDict, std::ostream& os);
    bool searchWord(std::map< std::string, std::vector< int > >& dictionary_, const std::string& word);

  private:
    std::map< std::string, std::vector< int > > dictionary_;
  };
}

#endif
