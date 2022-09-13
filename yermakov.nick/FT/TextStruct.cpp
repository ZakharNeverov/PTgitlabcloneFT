#include "TextStruct.hpp"

#include <fstream>
#include <sstream>

yermakov::Text::Text(CharData data, HuffmanTree tree):
  data_(data),
  isCompress_(false),
  tree_(tree)
{

}

yermakov::Text yermakov::compress(Text& text)
{
  Text newText(text.tree_.compress(text.data_), text.tree_);
  newText.isCompress_ = true;
  return newText;
}

yermakov::Text yermakov::decompress(Text& text)
{
  Text newText(text.tree_.decompress(text.data_), text.tree_);
  newText.isCompress_ = false;
  return newText;
}

std::ostream& yermakov::operator<<(std::ostream& input, Text& text)
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
