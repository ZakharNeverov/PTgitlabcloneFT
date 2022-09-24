#ifndef GLOBAL_LOCALE_HPP_FT
#define GLOBAL_LOCALE_HPP_FT

#include <locale>

namespace dobronravov
{
  struct globalLocale
  {
    globalLocale(const std::locale& loc);
    ~globalLocale();

  private:
    std::locale last_locale;
  };
}

#endif
