#include "Dictionary.hpp"
#include <ostream>
#include <utility>
#include <iomanip>
#include <numeric>
#include <type_traits>
#include "StreamGuard.hpp"

akimov::Dictionary::Dictionary(cnstStr& name):
  name_(name)
{}
bool akimov::Dictionary::operator==(const Dictionary& dict) const
{
  return name_ == dict.name_ && dictionary_ == dict.dictionary_;
}

std::string akimov::Dictionary::getName() const
{
  return name_;
}
std::size_t akimov::Dictionary::getSum() const
{
  if (empty())
  {
    return 0;
  }
  auto sumCount = [](std::size_t a, pairSS b)
  {
    return a + b.second;
  };
  std::size_t sum = std::accumulate(dictionary_.begin(), dictionary_.end(), 0, sumCount);
  return sum;
}
inline bool akimov::Dictionary::empty() const
{
  return dictionary_.empty();
}
void akimov::Dictionary::rename(cnstStr& name)
{
  name_ = name;
}
void akimov::Dictionary::print(std::ostream& stream) const
{
  if (empty())
  {
    return;
  }
  StreamGuard guard(stream);
  stream << name_ << '\n';
  for (auto i = dictionary_.begin(); i != dictionary_.end();)
  {
    printPair(stream, *i);
    stream << (++i != dictionary_.end() ? "\n" : "");
  }
}
void akimov::Dictionary::printPair(std::ostream& stream, const pairSS& pair) const
{
  const std::size_t sum = getSum();
  StreamGuard guard(stream);
  stream << std::fixed << std::setprecision(1);
  stream << pair.first << ": " << pair.second * 100.0 / sum << '%';
}
void akimov::Dictionary::printMostPopular(std::ostream& stream) const
{
  pairSS top[3]{};
  for (auto& i: dictionary_)
  {
    if (i.second > top[0].second)
    {
      top[2] = std::move(top[1]);
      top[1] = std::move(top[0]);
      top[0] = std::move(std::make_pair(i.first, i.second));
    }
    else if (i.second > top[1].second)
    {
      top[2] = std::move(top[1]);
      top[1] = std::move(std::make_pair(i.first, i.second));
    }
    else if (i.second > top[2].second)
    {
      top[2] = std::move(std::make_pair(i.first, i.second));
    }
  }
  StreamGuard guard(stream);
  stream << name_ << " 3 most popular:\n";
  for (std::size_t i = 0; i < 3; ++i)
  {
    if (!top[i].second)
    {
      break;
    }
    printPair(stream, top[i]);
    stream << (i != 2 && top[i + 1].second ? "\n" : "");
  }
}

void akimov::Dictionary::insert(cnstStr& word)
{
  auto iter = dictionary_.find(word);
  if (iter != dictionary_.end())
  {
    dictionary_[word]++;
  }
  else
  {
    dictionary_[word] = 1;
  }
}
void akimov::Dictionary::insertPair(const pairSS pair)
{
  if (search(pair.first) != end())
  {
    dictionary_[pair.first] += pair.second;
  }
  else
  {
    dictionary_[pair.first] = pair.second;
  }
}
akimov::mapCosntIter akimov::Dictionary::search(cnstStr& word) const
{
  return dictionary_.find(word);
}
akimov::mapIter akimov::Dictionary::search(akimov::cnstStr& word)
{
  return dictionary_.find(word);
}
void akimov::Dictionary::subtractPair(const pairSS pair)
{
  if (search(pair.first) == end())
  {
    return;
  }
  if (dictionary_[pair.first] > pair.second)
  {
    dictionary_[pair.first] -= pair.second;
  }
  else
  {
    deleteWord(pair.first);
  }
}
bool akimov::Dictionary::deleteWord(cnstStr& word)
{
  auto iter = dictionary_.find(word);
  if (iter != dictionary_.end())
  {
    dictionary_.erase(word);
    return true;
  }
  return false;
}
void akimov::Dictionary::drop()
{
  auto iter = dictionary_.begin();
  auto toDelete = iter;
  while (iter != dictionary_.end())
  {
    toDelete = iter;
    iter++;
    deleteWord(toDelete->first);
  }
}

akimov::mapCosntIter akimov::Dictionary::begin() const
{
  return dictionary_.begin();
}
akimov::mapCosntIter akimov::Dictionary::end() const
{
  return dictionary_.end();
}
akimov::mapIter akimov::Dictionary::begin()
{
  return dictionary_.begin();
}
akimov::mapIter akimov::Dictionary::end()
{
  return dictionary_.end();
}
