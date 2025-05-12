#include "init.h"
#include "rp2350/SIO.h"

extern "C" {
extern void _main();
}

namespace sys {

[[gnu::used]] [[gnu::retain]] [[noreturn]] void reset() {
  _main();
  __builtin_unreachable();
}

} // namespace sys