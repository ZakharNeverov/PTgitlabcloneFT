#ifndef GLOBAL_LOCALE_HPP_FT
#define GLOBAL_LOCALE_HPP_FT

#include <locale>

namespace kurzov
{
  struct globalLocale
  {
    globalLocale(const std::locale& loc);
    ~globalLocale();

  private:
    std::locale m_prev_loc;
  };
}

#endif
