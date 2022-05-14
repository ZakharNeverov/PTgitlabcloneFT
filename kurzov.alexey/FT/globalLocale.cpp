#include "globalLocale.hpp"

kurzov::globalLocale::globalLocale(const std::locale& loc):
  m_prev_loc(std::locale::global(loc))
{}

kurzov::globalLocale::~globalLocale()
{
  std::locale::global(m_prev_loc);
}
