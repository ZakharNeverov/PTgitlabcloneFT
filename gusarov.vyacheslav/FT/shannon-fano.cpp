#include "shannon-fano.hpp"
#include <functional>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include "io_struct.hpp"

namespace {
  bool isAddedСharacter(gusarov::CodeFano_t current, char ch)
  {
    return current.symbol == ch;
  }
  bool compareFrequency(gusarov::CodeFano_t left, gusarov::CodeFano_t right)
  {
    return left.freq > right.freq;
  }
  bool isCode(char ch)
  {
    return ch == '1' || ch == '0';
  }
  size_t getFreq(size_t l, size_t h, const gusarov::fanoAlphabet_t& fano)
  {
    size_t freq = 0;
    for (size_t i = l; i <= h - 1; i++) {
      freq += fano[i].freq;
    }
    return freq;
  }
}

gusarov::dict_t gusarov::getCode(const std::string& text)
{
  gusarov::fanoAlphabet_t fanoAlphabet = gusarov::getFanoAlphobet(text);
  gusarov::dict_t dict;

  for (auto iter : fanoAlphabet) {
    dict.insert(std::make_pair(iter.symbol, iter.code));
  }
  return dict;
}

std::string gusarov::getEncodedText(const std::string& text, dict_t dict)
{
  std::string encodedText = "";
  for (size_t i = 0; i < text.size(); ++i) {
    encodedText += dict.at(text[i]);
  }
  return encodedText;
}

std::string gusarov::getDecodedText(const std::string& text, dict_t dict)
{
  std::string decodedText = "";
  reverseDict_t reverseDict = getReverseDictionary(dict);
  std::string code = "";
  for (size_t i = 0; i < text.size(); ++i) {
    code += text[i];
    auto iter = reverseDict.find(code);
    if (iter != reverseDict.end()) {
      decodedText += iter->second;
      code = "";
    }
  }
  if (!code.empty()) {
    throw std::logic_error("unable to decode");
  }
  return decodedText;
}

size_t gusarov::getSize(const std::string& text)
{
  size_t size = std::count_if(text.begin(), text.end(), isCode);
  if (size == text.size()) {
    return static_cast< size_t >(std::ceil(size / 8));
  } else {
    return text.size();
  }
}

gusarov::fanoAlphabet_t gusarov::getFanoAlphobet(const std::string& text)
{
  fanoAlphabet_t fanoAlphabet = gusarov::getFrequency(text);
  std::sort(fanoAlphabet.begin(), fanoAlphabet.end(), compareFrequency);
  shannon(0, fanoAlphabet.size() - 1, fanoAlphabet);
  return fanoAlphabet;
}

void gusarov::shannon(size_t l, size_t h, gusarov::fanoAlphabet_t& fano)
{
  if ((l + 1) == h) {
    fano[h].code += '0';
    fano[l].code += '1';
    return;
  } else if (l == h || l > h) {
    return;
  } else {
    size_t diff1 = std::abs(static_cast< long >(getFreq(l, h, fano) - fano[h].freq));
    size_t j = 2;
    size_t k = 0;
    while (j != h - l + 1) {
      k = h - j;
      size_t diff2 = getFreq(l, k + 1, fano) - getFreq(k + 1, h, fano);
      if (diff2 >= diff1) {
        break;
      }
      diff1 = diff2;
      ++j;
    }
    ++k;
    for (size_t i = l; i <= k; ++i) {
      fano[i].code += '1';
    }
    for (size_t i = k + 1; i <= h; ++i) {
      fano[i].code += '0';
    }
    shannon(l, k, fano);
    shannon(k + 1, h, fano);
  }
}

gusarov::fanoAlphabet_t gusarov::getFrequency(const std::string& text)
{
  fanoAlphabet_t fanoAlphabet;
  for (size_t i = 0; i < text.size(); ++i) {
    auto pred = std::bind(isAddedСharacter, std::placeholders::_1, text[i]);
    auto iter = std::find_if(fanoAlphabet.begin(), fanoAlphabet.end(), pred);
    if (iter == fanoAlphabet.end()) {
      fanoAlphabet.push_back({text[i], 1});
    } else {
      iter->freq++;
    }
  }
  return fanoAlphabet;
}

gusarov::reverseDict_t gusarov::getReverseDictionary(dict_t dict)
{
  reverseDict_t reverseDict;
  for (auto iter : dict) {
    reverseDict[iter.second] = iter.first;
  }
  return reverseDict;
}

