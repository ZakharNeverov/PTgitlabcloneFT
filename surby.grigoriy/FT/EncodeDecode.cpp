#include "EncodeDecode.hpp"
#include <functional>
#include <numeric>

std::string surby::encode(std::string& text, std::string& dictionary)
{
  using namespace std::placeholders;

  std::unordered_map< char, std::string > dict;
  if (dictionary == "default")
  {
    std::string diction = surby::getBestDict(text);
    dict = surby::getDictionary(diction);
  }
  else
  {
    dict = surby::getDictionary(dictionary);
  }
  std::string result;
  std::for_each(text.begin(), text.end(), std::bind(encodeCompare, _1, std::ref(result), std::ref(dict)));
  return result;
}
std::string surby::decode(std::string& text, std::string& dictionary)
{
  using namespace std::placeholders;

  std::unordered_map< std::string, char > dict = surby::getReverseDictionary(dictionary);
  std::string code;
  std::string result;

  std::for_each(text.begin(), text.end(), std::bind(decodeCompare, _1, std::ref(result), std::ref(code), std::ref(dict)));

  return result;
}

void surby::decodeCompare(char symb, std::string& result, std::string& code, map_str_char& dict)
{
  code += symb;
  if (dict.find(code) != dict.end())
  {
    result += dict[code];
    code.clear();
  }
}
void surby::encodeCompare(char symb, std::string& result, std::unordered_map< char, std::string >& dict)
{
  result += dict[symb];
}

void surby::encodeHaffmanTree(Node* head, std::string str, std::unordered_map< char, std::string >& huffmanCode)
{
  if (head == nullptr)
  {
    return;
  }
  if (!head->left && !head->right) {
    huffmanCode[head->symb] = str;
  }
  surby::encodeHaffmanTree(head->left, str + "0", huffmanCode);
  surby::encodeHaffmanTree(head->right, str + "1", huffmanCode);
}
