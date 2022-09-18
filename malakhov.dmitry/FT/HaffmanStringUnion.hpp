#ifndef HAFFMAN_STRING_UNION_HPP
#define HAFFMAN_STRING_UNION_HPP

#include <memory>
#include <iostream>

#include "Haffman.hpp"

namespace malakhov
{
  class HaffmanStringUnion
  {
  public:
    HaffmanStringUnion() noexcept;
    HaffmanStringUnion(const HaffmanStringUnion& instance);
    HaffmanStringUnion(HaffmanStringUnion&& instance) noexcept;
    ~HaffmanStringUnion() noexcept;
    explicit HaffmanStringUnion(const std::string& str);
    explicit HaffmanStringUnion(const Haffman& haffman);

    bool isEncoded() const noexcept;
    const Haffman& getEncoded() const;
    const std::string& getDecoded() const;

    void set(const std::string& str);
    void set(const Haffman& haffman);

  private:
    union DecodedAndEncoded
    {
    public:
      DecodedAndEncoded() noexcept;
      Haffman* encoded;
      std::string* decoded;
    };

    void deleteOld() noexcept;

    DecodedAndEncoded stored_;
    bool isEncoded_;
  };

  std::ostream& operator<<(std::ostream& os, const HaffmanStringUnion& haffman);
  std::istream& operator>>(std::istream& is, HaffmanStringUnion& haffman);

  bool operator==(const HaffmanStringUnion& a, const HaffmanStringUnion& b);
}

#endif
