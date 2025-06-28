#pragma once

#include "rp2350/Common.h"

constexpr static u32 __bss_base = 0x20000400;
constexpr static u32 __bss_size = 63 << 10;

extern "C" {
[[gnu::used]] [[gnu::retain]] [[noreturn]] void start();
}
