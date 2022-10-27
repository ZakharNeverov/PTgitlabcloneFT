#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>

namespace
{
  std::invalid_argument invArgs("<Invalid arguments>");
  std::invalid_argument noFile("<No such file>");
  std::invalid_argument notOpenedFile("<File for output wasn't opened>");
  std::invalid_argument noText("<Text with this name doesn't exists>");
  std::invalid_argument textAlreadyExists("<Text with this name already exists>");
  std::invalid_argument invHufCode("<Given Huffman code is invalid>");
  std::invalid_argument invDecCode("<Decoding string must consist of only 0 or 1>");
  std::invalid_argument mergeTextNCode("<Merging encoded and not encoded text is forbidden>");
}

#endif
