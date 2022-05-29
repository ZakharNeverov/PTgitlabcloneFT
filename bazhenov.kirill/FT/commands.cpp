#include "commands.hpp"
#include <iostream>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iterator>
#include "messages.hpp"
#include "streamGuard.hpp"

namespace {
  using namespace std::placeholders;
  using dictionaryPair = std::pair< std::string, bazhenov::freq >;
  using pair = std::pair< std::string, int >;

  int getCountOfWord(int x, const dictionaryPair& dictionary, const std::string& word)
  {
    if (dictionary.second.find(word) != dictionary.second.end()) {
      return x + dictionary.second.at(word);
    }
    return x;
  }

  bool comparePairs(const pair& p1, const pair& p2)
  {
    if (p1.second == p2.second) {
      return p1.first.size() < p2.first.size();
    }
    return p1.second < p2.second;
  }

  bool isStartsWith(const pair& p, const std::string& prf)
  {
    return p.first.substr(0, prf.size()) == prf;
  }

  std::ostream& operator<<(std::ostream& os, const pair& p)
  {
    return os << p.first << " " << p.second;
  }

  template< typename Iterator >
  std::ostream& printDictionary (Iterator begin, Iterator end, std::ostream& out)
  {
    for (auto it = begin; it != end; ++it) {
      out << *it << "\n";
    }
    return out;
  }
}

bazhenov::print_t::print_t(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void bazhenov::print_t::operator()(bazhenov::dictionaries& storage)
{
  std::string name = "";
  in_ >> name;
  auto it = storage.find(name);
  if (it == storage.end()) {
    out_ << "There is no dictionary with this name!\n";
  } else {
    auto item = it->second;
    if (item.empty()) {
      bazhenov::printEmpty(std::cout) << "\n";
    }
    printDictionary(item.begin(), item.end(), out_);
  }
}

bazhenov::count_t::count_t(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void bazhenov::count_t::operator()(bazhenov::dictionaries& storage)
{
  std::string word = "";
  in_ >> word;
  auto bindFunction = std::bind(getCountOfWord, _1, _2, word);
  out_ << word << " " << std::accumulate(storage.begin(), storage.end(), 0, bindFunction) << "\n";
}

bazhenov::clear_t::clear_t(std::istream& in):
  in_(in)
{}

void bazhenov::clear_t::operator()(bazhenov::dictionaries& storage)
{
  std::string name = "";
  in_ >> name;
  auto it = storage.find(name);
  if (it != storage.end()) {
    it->second.clear();
  }
}

bazhenov::find_t::find_t(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void bazhenov::find_t::operator()(bazhenov::dictionaries& storage)
{
  std::string word = "";
  in_ >> word;
  for (auto& it: storage) {
    auto desiredString = it.second.find(word);
    if (desiredString != it.second.end()) {
      out_ << it.first << " " << *desiredString << "\n";
    }
  }
}

bazhenov::upper_t::upper_t(std::istream& in, std::ostream& out, std::string extreme):
  in_(in),
  out_(out),
  extreme_(std::move(extreme))
{}

void bazhenov::upper_t::operator()(bazhenov::dictionaries& storage)
{
  std::string line = "";
  in_ >> line;
  auto it = storage.find(line);
  if (it != storage.end()) {
    auto item = storage.at(line);
    pair result;
    if (extreme_ == "most") {
      result = *std::max_element(item.begin(), item.end(), comparePairs);
    } else {
      result = *std::min_element(item.begin(), item.end(), comparePairs);
    }
    out_ << result << "\n";
  } else {
    bazhenov::printNotExist(out_) << "\n";
  }
}

bazhenov::search_pref_t::search_pref_t(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void bazhenov::search_pref_t::operator()(bazhenov::dictionaries& storage)
{
  std::string name = "";
  std::string prf = "";
  in_ >> name >> prf;
  auto it = storage.find(name);
  if (it != storage.end()) {
    std::map< std::string, int > pairs;
    auto isStarts = std::bind(isStartsWith, _1, prf);
    auto item = it->second;
    std::copy_if(item.begin(), item.end(), std::inserter(pairs, pairs.begin()), isStarts);
    if (pairs.empty()) {
      bazhenov::printNoMatches(out_) << "\n";
    }
    printDictionary(pairs.begin(), pairs.end(), out_);
  } else {
    bazhenov::printNotExist(out_) << "\n";
  }
}

bazhenov::search_t::search_t(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void bazhenov::search_t::operator()(bazhenov::dictionaries& storage)
{
  std::string name = "";
  in_ >> name;
  auto it = storage.find(name);
  if (it != storage.end()) {
    std::map< std::string, int > pairs;
    std::string word = "";
    while (in_.peek() != '\n') {
      in_ >> word;
      auto item = it->second.find(word);
      if (item != it->second.end()) {
        pairs.emplace(*item);
      }
    }
    if (pairs.empty()) {
      bazhenov::printNoMatches(out_) << "\n";
    }
    printDictionary(pairs.begin(), pairs.end(), out_);
  } else {
    bazhenov::printNotExist(out_) << "\n";
    in_.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}

bazhenov::add_t::add_t(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void bazhenov::add_t::operator()(bazhenov::dictionaries& storage)
{
  std::string name = "";
  in_ >> name;
  auto it = storage.find(name);
  std::string word = "";
  if (it != storage.end()) {
    while (in_.peek() != '\n') {
      in_ >> word;
      bazhenov::readWord(word, it->second);
    }
  } else {
    freq dictionary;
    while (in_.peek() != '\n') {
      in_ >> word;
      bazhenov::readWord(word, dictionary);
    }
    storage.insert(std::make_pair(name, dictionary));
  }
}

bazhenov::diff_t::diff_t(std::istream& in, std::ostream& out):
  in_(in),
  out_(out)
{}

void bazhenov::diff_t::operator()(bazhenov::dictionaries& storage)
{
  std::string name1 = "", name2 = "";
  in_ >> name1 >> name2;
  auto it1 = storage.find(name1);
  auto it2 = storage.find(name2);

  if (it1 != storage.end() && it2 != storage.end()) {
    std::map< std::string , int> diffs;
    auto tmp1 = it1->second.begin();
    while (tmp1 != it1->second.end()) {
      auto equal = it2->second.find(tmp1->first);
      if (equal != it2->second.end()) {
        diffs.emplace(tmp1->first, tmp1->second - equal->second);
      }
      ++tmp1;
    }
    bazhenov::StreamGuard guard(out_);
    out_ << std::showpos;
    printDictionary(diffs.begin(), diffs.end(), out_);
    out_.clear();
  } else {
    bazhenov::printNotExist(out_) << "\n";
  }
}
