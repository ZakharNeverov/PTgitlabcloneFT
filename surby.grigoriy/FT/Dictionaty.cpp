#include "Dictionary.hpp"
#include <iostream>

std::string surby::getBestDict(std::string& text)
{
  std::unordered_map< char, int > dict;
  for (char simv : text) {
    dict[simv]++;
  }

  std::priority_queue< Node*, std::vector< Node* >, compare > pq;

  for (auto pair : dict) {
    pq.push(surby::getNode(pair.first, pair.second, nullptr, nullptr));
  }

  while (pq.size() != 1)
  {
    Node* left = pq.top();
    pq.pop();
    Node* right = pq.top();
    pq.pop();
    int sum = left->count + right->count;
    pq.push(surby::getNode('\0', sum, left, right));
  }

  Node* head = pq.top();

  std::unordered_map< char, std::string > huffmanCode;
  surby::encodeHaffmanTree(head, "", huffmanCode);

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

std::unordered_map< char, std::string > surby::getDictionary(std::string& line)
{
  std::unordered_map< char, std::string > dict;

  while (line.size() > 1)
  {
    std::string element = surby::getNextArg(line, 1);

    size_t i = element.size();

    if (i == 1)
    {
      std::string cipher = surby::getNextArg(line, 1);
      dict[element[0]] = cipher;
    }
    else if (i == 2 && element == "\\n")
    {
      dict['\n'] = surby::getNextArg(line, 1);
    }
    else
    {
      throw std::overflow_error("wrong fwd\n");
    }
  }
  return dict;
}

std::unordered_map<std::string, char> surby::getReverseDictionary(std::string& line)
{
  std::unordered_map<std::string, char> revdict;

  while (line.size() > 1)
  {
    std::string element = surby::getNextArg(line, 1);

    size_t i = element.size();

    if (i == 1)
    {
      std::string cipher = surby::getNextArg(line, 1);
      revdict[cipher] = element[0];
    }
    else if (i == 2 && element == "\\n")
    {
      revdict[surby::getNextArg(line, 1)] = '\n';
    }
    else
    {
      throw std::overflow_error("wrong fwd\n");
    }
  }
  return revdict;
}
