#include "TextStruct.hpp"

#include <fstream>

yermakov::Text::Text():
  data_(),
  tree_(),
  isCompress_(false)
{
}

yermakov::Text::Text(const CharData& data, const HuffmanTree& tree):
  data_(data),
  tree_(tree),
  isCompress_(false)
{
}

yermakov::Text yermakov::compress(const Text& text)
{
  Text newText(text.tree_.compress(text.data_), text.tree_);
  newText.isCompress_ = true;
  return newText;
}

yermakov::Text yermakov::decompress(const Text& text)
{
  Text newText(text.tree_.decompress(text.data_), text.tree_);
  newText.isCompress_ = false;
  return newText;
}

std::ostream& yermakov::operator<<(std::ostream& input, const Text& text)
{
  input << "LANGUAGE: " << text.data_.language_ << "\n";
  input << "TEXT: " << text.data_.text_ << "\n";
  return input;
}

std::istream& yermakov::operator>>(std::istream& in, yermakov::Text& text)
{
  in >> text.data_;
  text.tree_ = HuffmanTree(text.data_);
  return in;
}
