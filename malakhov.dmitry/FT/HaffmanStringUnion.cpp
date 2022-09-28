#include "HaffmanStringUnion.hpp"

#include <sstream>
#include <limits>

namespace
{
  const char* const WRONG_TYPE_ERR = "Wrong type access attempt";
}

malakhov::HaffmanStringUnion::DecodedAndEncoded::DecodedAndEncoded() noexcept:
  decoded()
{}

malakhov::HaffmanStringUnion::DecodedAndEncoded::~DecodedAndEncoded() noexcept
{}

malakhov::HaffmanStringUnion::HaffmanStringUnion(const HaffmanStringUnion& instance):
  stored_(),
  isEncoded_(instance.isEncoded_)
{
  if (isEncoded_)
  {
    stored_.encoded = instance.stored_.encoded;
  }
  else
  {
    stored_.decoded = instance.stored_.decoded;
  }
}

malakhov::HaffmanStringUnion::HaffmanStringUnion(HaffmanStringUnion&& instance) noexcept:
  stored_(),
  isEncoded_(instance.isEncoded_)
{
  if (isEncoded_)
  {
    stored_.encoded = std::move(instance.stored_.encoded);
  }
  else
  {
    stored_.decoded = std::move(instance.stored_.decoded);
  }
}

malakhov::HaffmanStringUnion::HaffmanStringUnion() noexcept:
  stored_(),
  isEncoded_(false)
{}

malakhov::HaffmanStringUnion::~HaffmanStringUnion() noexcept
{
  if (isEncoded_)
  {
    stored_.encoded.~Haffman();
    return;
  }

  using std::string;
  stored_.decoded.~string();
}

malakhov::HaffmanStringUnion::HaffmanStringUnion(const std::string& str):
  stored_(),
  isEncoded_(false)
{
  stored_.decoded = str;
}

malakhov::HaffmanStringUnion::HaffmanStringUnion(const Haffman& haf):
  stored_(),
  isEncoded_(true)
{
  stored_.encoded = haf;
}

bool malakhov::HaffmanStringUnion::isEncoded() const noexcept
{
  return isEncoded_;
}

const malakhov::Haffman& malakhov::HaffmanStringUnion::getEncoded() const
{
  if (!isEncoded_)
  {
    throw std::logic_error(WRONG_TYPE_ERR);
  }
  return stored_.encoded;
}

const std::string& malakhov::HaffmanStringUnion::getDecoded() const
{
  if (isEncoded_)
  {
    throw std::logic_error(WRONG_TYPE_ERR);
  }
  return stored_.decoded;
}

void malakhov::HaffmanStringUnion::set(const std::string& str)
{
  stored_.decoded = str;
  isEncoded_ = false;
}

void malakhov::HaffmanStringUnion::set(const Haffman& haf)
{
  stored_.encoded = haf;
  isEncoded_ = true;
}

std::ostream& malakhov::operator<<(std::ostream& os, const HaffmanStringUnion& instance)
{
  std::ostream::sentry s{os};
  if (!s)
  {
    return os;
  }
  return instance.isEncoded() ? os << instance.getEncoded() : os << instance.getDecoded();
}

std::istream& malakhov::operator>>(std::istream& is, HaffmanStringUnion& instance)
{
  const static char delim = std::numeric_limits< char >::max();

  std::istream::sentry s(is);
  if (!s)
  {
    return is;
  }
  std::stringstream buf;
  buf << is.rdbuf();
  const std::string text = buf.str();
  const size_t firstBrace = text.find(delim);
  if (firstBrace == std::string::npos) // string is decoded
  {
    instance.set(text);
    return is;
  }

  const size_t secondBrace = text.find(delim, firstBrace + 1);
  if (secondBrace != std::string::npos && text[secondBrace + 1] == ' ') // string is encoded
  {
    const std::string rawBitmask = text.substr(firstBrace + 1, secondBrace - firstBrace - 1);
    if (rawBitmask.length() % 2)
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }

    Haffman::BitmaskT bitmask;
    for (size_t i = 0; i < rawBitmask.length() / 2; ++i)
    {
      bitmask.insert({rawBitmask[i * 2], rawBitmask[i * 2 + 1]});
    }
    const std::string str = text.substr(secondBrace + 2, str.length() - secondBrace - 2);
    instance.set({bitmask, str});
  }
  else // string is invalid
  {
    is.setstate(std::ios_base::failbit);
  }
  return is;
}

bool malakhov::operator==(const HaffmanStringUnion& a, const HaffmanStringUnion& b)
{
  if (a.isEncoded() && b.isEncoded())
  {
    return a.getEncoded() == b.getEncoded();
  }
  else if (!(a.isEncoded() || b.isEncoded()))
  {
    return a.getDecoded() == b.getDecoded();
  }
  return false;
}
