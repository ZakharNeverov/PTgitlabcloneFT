#include "Dictionary.hpp"
#include <numeric>
#include <functional>
#include <iterator>
#include <iostream>

std::string surby::getBestDict(std::string& text)
{
  using namespace std::placeholders;
  std::unordered_map< char, int > dict;
  std::for_each(text.begin(), text.end(), std::bind(surby::createDictCompare, _1, std::ref(dict)));
  surby::priorityQueue pq;
  std::for_each(dict.begin(), dict.end(), std::bind(createPriorityQueueCompare, _1, std::ref(pq)));

  while (pq.size() > 1)
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
  std::string result;
  result = std::accumulate(huffmanCode.begin(), huffmanCode.end(), std::string(), surby::getBestDictCompare);
  return result;
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
std::unordered_map< std::string, char > surby::getReverseDictionary(std::string& line)
{
  std::unordered_map< std::string, char > revdict;

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

std::string surby::getBestDictCompare(std::string result, std::pair< char, std::string > pair)
{
  if (pair.first == '\n')
  {
    result += "\\n";
  }
  else
  {
    result += pair.first;
  }
  result += " ";
  result += pair.second;
  result += '\n';
  return result;
}
void surby::createPriorityQueueCompare(std::pair< const char, int > pair, priorityQueue& pq)
{
  pq.push(surby::getNode(pair.first, pair.second, nullptr, nullptr));
}
void surby::createDictCompare(char symb, std::unordered_map< char, int >& dict)
{
  dict[symb]++;
}
