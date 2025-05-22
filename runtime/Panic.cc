#include "runtime/Panic.h"
#include "RP2350/Common.h"
#include "runtime/String.h"

extern "C" {
[[gnu::noinline]] [[noreturn]]
void __panic(cxx::String type, cxx::String msg, cxx::String locStr, u32 arg1, u32 arg2, u32 arg3) {
  (void)type;
  (void)msg;
  (void)locStr;
  (void)arg1;
  (void)arg2;
  (void)arg3;
  while (true) { _BUSY_LOOP(); };
}
}
