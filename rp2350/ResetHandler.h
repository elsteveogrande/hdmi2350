#pragma once

#include "rp2350/Common.h"

constexpr static u32 __bss_base = 0x20001000;
constexpr static u32 __bss_size = 60 << 10;

extern "C" {
[[gnu::used]] [[gnu::retain]] [[noreturn]] void start();
}
