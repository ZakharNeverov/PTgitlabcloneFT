#include "globalLocale.hpp"

dobronravov::globalLocale::globalLocale(const std::locale& loc):
  last_locale(std::locale::global(loc))
{}

dobronravov::globalLocale::~globalLocale()
{
  std::locale::global(last_locale);
}
