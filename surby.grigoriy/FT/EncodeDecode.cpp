#include "EncodeDecode.hpp"
#include <iostream>
#include <unordered_map>
#include <queue>

std::string getBestDict(std::string& text)
{
  std::unordered_map<char, int> freq;
  for (char ch : text) {
    freq[ch]++;
  }

  std::priority_queue<Node*, std::vector<Node*>, comp> pq;

  for (auto pair : freq) {
    pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
  }

  while (pq.size() != 1)
  {
    Node* left = pq.top();
    pq.pop();
    Node* right = pq.top();
    pq.pop();
    int sum = left->freq + right->freq;
    pq.push(getNode('\0', sum, left, right));
  }

  Node* root = pq.top();

  std::unordered_map<char, std::string> huffmanCode;
  encode(root, "", huffmanCode);

  std::string fwd;
  for (auto pair : huffmanCode) {

    if (pair.first == '\n')
    {
      fwd += "\\n";
    }
    else
    {
      fwd += pair.first;
    }
    fwd += " ";
    fwd += pair.second;
    fwd += '\n';
  }
  return fwd;
}


std::string encode(std::string& text, std::string& dictionary)
{
  std::unordered_map<char, std::string> dict;
  if (dictionary == "default")
  {
    std::string diction = getBestDict(text);
    dict = getDictionary(diction);
  }
  else
  {
    dict = getDictionary(dictionary);
  }

  std::string result = "";
  for (char ch : text) {
    result += dict[ch];
  }
  return result;
}


std::unordered_map<char, std::string> getDictionary(std::string& line)
{
  std::unordered_map<char, std::string> dict;

  while (line.size() > 1)
  {
    std::string element = getNextArg(line, 1);

    size_t i = element.size();

    if (i == 1)
    {
      std::string cipher = getNextArg(line, 1);
      dict[element[0]] = cipher;
    }
    else if (i == 2 && element == "\\n")
    {
      dict['\n'] = getNextArg(line, 1);
    }
    else
    {
      throw std::overflow_error("wrong fwd\n");
    }
  }
  return dict;
}


std::unordered_map<std::string, char> getReverseDictionary(std::string& line)
{
  std::unordered_map<std::string, char> revdict;

  while (line.size() > 1)
  {
    std::string element = getNextArg(line, 1);

    size_t i = element.size();

    if (i == 1)
    {
      std::string cipher = getNextArg(line, 1);
      revdict[cipher] = element[0];
    }
    else if (i == 2 && element == "\\n")
    {
      revdict[getNextArg(line, 1)] = '\n';
    }
    else
    {
      throw std::overflow_error("wrong fwd\n");
    }
  }
  return revdict;


}












std::string decode(std::string& text, std::string& dictionary)
{
  std::unordered_map<std::string, char> dict = getReverseDictionary(dictionary);

  std::string code;
  std::string result;

  for (char simv : text)
  {
    code += simv;
    if (dict.find(code) != dict.end())
    {
      result += dict[code];
      code.clear();
    }
  }

  return result;
}
