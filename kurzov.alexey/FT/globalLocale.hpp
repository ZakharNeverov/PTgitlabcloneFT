#ifndef GLOBAL_LOCALE_HPP
#define GLOBAL_LOCALE_HPP

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
