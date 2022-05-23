#include "Utilities.hpp"
#include "Dictionary.hpp"

namespace shkroba
{

  std::ostream &operator<<(std::ostream& out, std::set< std::string >& set)
  {
    for (auto &item: set)
    {
      out << item << ' ';
    }
    out << '\n';
    return out;
  }

  Dictionary createCommonDictionary(std::vector<Dictionary> &common)
  {
    Dictionary result;
    bool isCommon;

    std::map< std::string, size_t > translates;

    Dictionary dictionary = *common.begin();
    for (const auto& pair: dictionary)
    {
      isCommon = true;
      translates.clear();
      for (const auto &dictionarySecond: common)
      {
        if (dictionarySecond.search(pair.first) == dictionarySecond.end())
        {
          isCommon = false;
          break;
        }
        else
        {
          pairER item = *dictionarySecond.search(
            pair.first);
          for (const auto &word: *item.second)
          {
            translates[word]++;
          }
        }
      }
      if (isCommon)
      {
        std::set<std::string> set;
        for (const auto &translate: translates)
        {
          if (translate.second == common.size())
          {
            set.insert(translate.first);
          }
        }
        if (set.empty())
        {
          std::cout << "Write translate for " + pair.first << '\n';
          std::cout << "Write \"Exit\" to escape\n";
          std::string in;
          while (true)
          {
            std::cin >> in;
            if (in == "Exit" && set.empty())
            {
              continue;
            }
            if (in == "Exit")
            {
              break;
            }
            set.insert(in);
          }
        }
        std::shared_ptr<std::set<std::string> > sharedPtr = std::make_shared<std::set<std::string> >(set);
        result.insert({pair.first, sharedPtr});
      }
    }
    return result;
  }

  void doCommonDictionary(const std::vector<Dictionary> &base, size_t n)
  {
    if (n > 5 || n < 1)
    {
      throw std::invalid_argument("Error! There are only 5 dictionaries.");
    }
    std::vector<Dictionary> dictionaries;

    for (int i = 0; i < n; i++)
    {
      dictionaries.push_back(base[i]);
    }
    createCommonDictionary(dictionaries).printDictionary(std::cout);
  }

  Dictionary createFromUniqueWords(const Dictionary &d1, const Dictionary &d2)
  {
    Dictionary common;
    for (const pairER &item: d1)
    {
      if (d2.search(item.first) == d2.end())
      {
        common.insert(item);
      }
    }
    for (const pairER &item: d2)
    {
      if (d1.search(item.first) == d1.end())
      {
        common.insert(item);
      }
    }
    return common;
  }

  Dictionary createFromOneTranslate(const Dictionary &dictionary)
  {
    Dictionary newDictionary;
    for (const auto &word: dictionary)
    {
      if (word.second->size() == 1)
      {
        newDictionary.insert(word);
      }
    }
    return newDictionary;
  }

  void doPrintDictionary(const Dictionary &dictionary, std::ostream &out)
  {
    dictionary.printDictionary(out);
  }

  void doSize(const Dictionary &dictionary, std::ostream &out)
  {
    out << dictionary.size();
  }

  void doFindWord(const Dictionary &dictionary, char letter, std::ostream &out)
  {
    dictionary.findWord(letter, std::cout);
  }

  void doAddWordsFromAnother(Dictionary &source, const Dictionary &extra, std::ostream &out)
  {
    source.addWordsFromAnother(extra);
    source.printDictionary(out);
  }

  void doCreateFromOneTranslate(const Dictionary &dictionary, std::ostream &out)
  {
    createFromOneTranslate(dictionary).printDictionary(out);
  }

  void doCreateFromUniqueWords(const Dictionary &first, const Dictionary &second, std::ostream &out)
  {
    createFromUniqueWords(first, second).printDictionary(out);
  }

  void help(std::ostream &out)
  {
    out << "The range of commands" << '\n' << '\n' << '\n';
    out << "doPrintDictionary" << '\n';
    out << "Print words from the selected dictionary." << '\n' << '\n';

    out << "doSize" << '\n';
    out << "Print size of selected dictionary." << '\n' << '\n';

    out << "doFindWord" << '\n';
    out << "Print words beginning with input letter from the selected dictionary." << '\n' << '\n';

    out << "doAddWordsFromAnother" << '\n';
    out << "Add words from the second dictionary to the first and print it." << '\n' << '\n';

    out << "doCreateFromOneTranslate" << '\n';
    out << "Create the dictionary and print words from the selected dictionary that have one translation.";
    out << '\n' << '\n';

    out << "doCreateFromUniqueWords" << '\n';
    out << "Create common dictionary from words that do not repeat in two selected dictionaries and print it.";
    out << '\n' << '\n';

    out << "doCommonDictionary" << '\n';
    out << "Create common dictionary with input number of dictionaries and print it." << '\n';
    out << "There are only 5 dictionaries. Do not input number more than five. " << '\n' << '\n';
  }

  void createTestDictionaries(Dictionary &d1, Dictionary &d2, Dictionary &d3, Dictionary &d4, Dictionary &d5)
  {
    d1.insert("Cat", "Киса");
    d1.insert("Cat", "Кис");
    d1.insert("Moon", "Луна");
    d1.insert("Wind", "Ветер");
    d1.insert("Sorrow", "Тоска");
    d1.insert("Laces", "Шнурки");
    d1.insert("War", "Война");
    d1.insert("Peace", "Мир");
    d1.insert("Dog", "Собака");
    d1.insert("Worm", "Червяк");

    d2.insert("Cat", "Киса");
    d2.insert("Cat", "Кис");
    d2.insert("Freedom", "Свобода");
    d2.insert("Slavery", "Рабство");
    d2.insert("Sorrow", "Печаль");
    d2.insert("Cathedral", "Собор");
    d2.insert("Cat", "Кисонька");
    d2.insert("Dog", "Собака");
    d2.insert("Bread", "Хлеб");
    d2.insert("Crow", "Ворона");

    d3.insert("Cat", "Кошка");
    d3.insert("Cat", "Котя");
    d3.insert("Ignorance", "Незнание");
    d3.insert("Strength", "Сила");
    d3.insert("Sorrow", "Грусть");
    d3.insert("Sorrow", "Печаль");
    d3.insert("Moon", "Луна");
    d3.insert("Cat", "Киса");
    d3.insert("Dog", "Собака");
    d3.insert("Cookie", "Печенье");

    d4.insert("Cat", "Кошка");
    d4.insert("Worm", "Червяк");
    d4.insert("Stream", "Поток");
    d4.insert("Sorrow", "Грусть");
    d4.insert("Dog", "Собака");
    d4.insert("Picture", "Картинка");

    d5.insert("Something", "Что-то");
    d5.insert("in", "На");
    d5.insert("Way", "Пути");
    d5.insert("Stream", "Поток");
    d5.insert("Dog", "Собака");
    d5.insert("Laces", "Шнурки");
    d5.insert("Sorrow", "Скорбь");
  }

  void testCommandSystem(Dictionary &d1, Dictionary &d2, Dictionary &d3, Dictionary &d4, Dictionary &d5)
  {
    std::vector<shkroba::Dictionary> base;
    base.push_back(d1);
    base.push_back(d2);
    base.push_back(d3);
    base.push_back(d4);
    base.push_back(d5);

    // Command System

    //shkroba::help(std::cout);

    shkroba::doPrintDictionary(d1, std::cout);
    std::cout << '\n' << '\n';

    shkroba::doSize(d3, std::cout);
    std::cout << '\n' << '\n';

    char letter = 'C';
//    std::cin >> letter;
    shkroba::doFindWord(d2, letter, std::cout);
    std::cout << '\n' << '\n';

    shkroba::doAddWordsFromAnother(d4, d1, std::cout);
    std::cout << '\n' << '\n';

    doCreateFromOneTranslate(d1, std::cout);
    std::cout << '\n' << '\n';

    shkroba::doCreateFromUniqueWords(d3, d5, std::cout);
    std::cout << '\n' << '\n';

    size_t dictionariesNumber;
    std::cin >> dictionariesNumber;
    shkroba::doCommonDictionary(base, dictionariesNumber);

  }

}
