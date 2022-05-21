#ifndef ENCODEDECODE_HPP
#define ENCODEDECODE_HPP
#include <string>
#include <unordered_map>
#include "Haffman.hpp"
#include "Delimiter.hpp"

std::string getBestDict(std::string&);

std::unordered_map<char, std::string> getDictionary(std::string& line);

std::string encode(std::string&, std::string&);

std::string decode(std::string&, std::string&);

std::unordered_map<std::string, char> getReverseDictionary(std::string& line);
#endif
