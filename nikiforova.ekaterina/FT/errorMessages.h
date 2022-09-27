#ifndef ERRORMESSAGES_H
#define ERRORMESSAGES_H
#include <ostream>

namespace nikiforova {
  std::ostream& invalidCommandMessage(std::ostream& out);
  std::ostream& emptyMessage(std::ostream& out);
}
#endif
