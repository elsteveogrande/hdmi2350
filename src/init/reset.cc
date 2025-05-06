#include "init.h"

namespace sys {

[[gnu::used]] [[gnu::retain]] [[noreturn]] void reset() { while (1); }

} // namespace sys