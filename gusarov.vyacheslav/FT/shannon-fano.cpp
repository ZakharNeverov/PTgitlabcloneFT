#include "shannon-fano.hpp"
#include <fstream>
#include <functional>
#include <algorithm>
#include "IOfile.hpp"

namespace {
  bool isCorrectSymbol(gusarov::CodeFano_t current, char ch)
  {
    return current.symbol == ch;
  }
  bool compareCodeFano(gusarov::CodeFano_t left, gusarov::CodeFano_t right)
  {
    return left.freq > right.freq;
  }
  bool isCode(std::string code)
  {
    for (size_t i = 0; i < code.size(); i++) {
      if (code[i] != '1' && code[i] != '0') {
        return false;
      }
    }
    return true;
  }
}

gusarov::fanoAlphabet_t gusarov::getFanoAlphobet(std::ifstream& file)
{
  fanoAlphabet_t fanoAlphabet = gusarov::getFrequencyForCode(file);
  std::sort(fanoAlphabet.begin(), fanoAlphabet.end(), compareCodeFano);
  shannon(0, fanoAlphabet.size() - 1, fanoAlphabet);
  return fanoAlphabet;
}

void gusarov::shannon(size_t l, size_t h, gusarov::fanoAlphabet_t& fanoAlphabet)
{
  {
    size_t pack1 = 0, pack2 = 0, diff1 = 0, diff2 = 0;
    if ((l + 1) == h || l == h || l > h) {
      if (l == h || l > h) {
        return;
      }
      fanoAlphabet[h].code += '0';
      fanoAlphabet[l].code += '1';
      return;
    } else {
      for (size_t i = l; i <= h - 1; i++) {
        pack1 += fanoAlphabet[i].freq;
      }
      pack2 += fanoAlphabet[h].freq;
      diff1 = pack1 - pack2;
      if (diff1 < 0) {
        diff1 *= (-1);
      }
      size_t j = 2;
      size_t k = 0;
      while (j != h - l + 1) {
        k = h - j;
        pack1 = pack2 = 0;
        for (size_t i = l; i <= k; i++) {
          pack1 += fanoAlphabet[i].freq;
        }
        for (size_t i = h; i > k; i--) {
          pack2 += fanoAlphabet[i].freq;
        }
        diff2 = pack1 - pack2;
        if (diff2 >= diff1) {
          break;
        }
        diff1 = diff2;
        j++;
      }
      k++;
      for (size_t i = l; i <= k; i++) {
        fanoAlphabet[i].code += '1';
      }
      for (size_t i = k + 1; i <= h; i++) {
        fanoAlphabet[i].code += '0';
      }
      shannon(l, k, fanoAlphabet);
      shannon(k + 1, h, fanoAlphabet);
    }
  }
}

gusarov::fanoAlphabet_t gusarov::getFrequencyForCode(std::ifstream& file)
{
  {
    fanoAlphabet_t fanoAlphabet;
    while (!file.eof()) {
      char ch = file.get();
      if (file.eof()) {
        break;
      }
      auto pred = std::bind(isCorrectSymbol, std::placeholders::_1, ch);
      auto iter = std::find_if(fanoAlphabet.begin(), fanoAlphabet.end(), pred);
      if (iter == fanoAlphabet.end()) {
        fanoAlphabet.push_back({ch, 1});
      } else {
        iter->freq++;
      }
    }
    file.close();
    return fanoAlphabet;
  }
}

gusarov::dict_t gusarov::getDictionary(std::ifstream& fileCode)
{
  dict_t fano;
  while (!fileCode.eof()) {
    std::string str = "";
    fileCode >> str;
    char ch = getSymbol(str);
    std::string code;
    fileCode >> code;
    if (!isCode(code)) {
      fileCode.close();
      throw std::invalid_argument("Incorrect code");
    }
    fano.insert(std::make_pair(ch, code));
  }
  return fano;
}

gusarov::reverseDict_t gusarov::getReverseDictionary(std::ifstream& fileCode)
{
  dict_t dict = getDictionary(fileCode);
  reverseDict_t reverseDict;
  for (dict_t::iterator it = dict.begin(); it != dict.end(); ++it) {
    reverseDict[it->second] = it->first;
  }
  return reverseDict;
}

void gusarov::doEncode(std::ifstream& in, std::ofstream& out, dict_t fano)
{
  while (!in.eof()) {
    char ch = in.get();
    if (in.eof()) {
      break;
    }
    auto commandIter = fano.find(ch);
    if (commandIter != fano.end()) {
      out << commandIter->second;
    } else {
      in.close();
      out.close();
      throw std::invalid_argument("Unknown character");
    }
  }
}

void gusarov::doDecode(std::ifstream& in, std::ofstream& out, reverseDict_t fano)
{
  std::string code = "";
  while (!in.eof()) {
    char ch = in.get();
    if (in.eof()) {
      break;
    }
    code += ch;
    auto commandIter = fano.find(code);
    if (commandIter != fano.end()) {
      out << commandIter->second;
      code = "";
    }
  }
  if (!code.empty()) {
    in.close();
    out.close();
    throw std::invalid_argument("Incorrect code");
  }
}

void gusarov::createCodeFano(std::ifstream& in, std::ofstream& out)
{
  fanoAlphabet_t fanoAlphabet = getFanoAlphobet(in);
  fanoAlphabet_t::iterator iter = fanoAlphabet.begin();
  while (iter != fanoAlphabet.end()) {
    out << getSymbol((*iter).symbol) << " " << (*iter).code << "\n";
    iter++;
  }
}
