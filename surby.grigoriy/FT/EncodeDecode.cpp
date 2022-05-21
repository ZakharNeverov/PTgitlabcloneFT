#include "EncodeDecode.hpp"

std::string surby::encode(std::string& text, std::string& dictionary)
{
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

  std::string result = "";
  for (char simv : text) {
    result += dict[simv];
  }
  return result;
}

std::string surby::decode(std::string& text, std::string& dictionary)
{
  std::unordered_map< std::string, char > dict = surby::getReverseDictionary(dictionary);

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

void surby::encodeHaffmanTree(Node* head, std::string str, std::unordered_map< char, std::string >& huffmanCode)
{
  if (head == nullptr)
    return;
  if (!head->left && !head->right) {
    huffmanCode[head->simv] = str;
  }
  surby::encodeHaffmanTree(head->left, str + "0", huffmanCode);
  surby::encodeHaffmanTree(head->right, str + "1", huffmanCode);
}
