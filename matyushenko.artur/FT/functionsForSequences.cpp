#include "functionsForSequences.hpp"
namespace matyushenko
{
  std::string findLongestSub(str &name1, str &name2)
  {
    std::string result;
    std::string finalResult;
    dictionary resultD{};
    auto memory = name2.begin();
    auto first = name1.begin();
    size_t size_ = 0;
    while (name1.size() != size_)
    {
      while (first != name1.end())
      {
        for (auto second = memory; second != name2.end(); second++)
        {
          if (*first == *second)
          {
            memory = ++second;
            result += *first;
            break;
          }
        }
        first++;
      }
      if (finalResult.length() <= result.length())
      {
        finalResult = result;
      }
      result = "";
      memory = name2.begin();
      size_++;
      first = name1.begin() + size_;
    }
    if (finalResult.length() <= result.length())
    {
      finalResult = result;
    }
    finalResult = "*" + finalResult;
    return finalResult;
  }
}
