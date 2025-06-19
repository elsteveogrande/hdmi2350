#pragma once

#include "runtime/Panic.h"

extern u8* __bss_base;
extern u32 __bss_size;

extern "C" {
[[gnu::used]] [[gnu::retain]] [[noreturn]] void start();
}
