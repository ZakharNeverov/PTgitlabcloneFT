#ifndef HAFFMAN_HPP
#define HAFFMAN_HPP

#include <ostream>
#include <string>

#include "Dictionary.hpp"

namespace malakhov
{
  class Haffman
  {
  public:
    using BitmaskT = Dictionary< unsigned char, unsigned char >;

    Haffman(const BitmaskT& bitmask, const std::string& encoded);
    static Haffman produce(const std::string& decoded);

    std::string decode() const;
    const std::string& getEncoded() const noexcept;
    const BitmaskT& getBitmask() const noexcept;

  private:
    std::string encoded_;
    BitmaskT bitmask_;
  };

  bool operator==(const Haffman& op1, const Haffman& op2);
  std::ostream& operator<<(std::ostream& os, const Haffman& haffman);
}

#endif
