#include "rp2350/Common.h"
#include "runtime/String.h"

extern "C" {
[[gnu::noinline]] [[noreturn]]
void __panic(cxx::String type, cxx::String msg, cxx::String locStr, u32 arg1, u32 arg2, u32 arg3);
}
