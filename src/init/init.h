#pragma once

#include "../types.h"

extern "C" {
extern void const* __stack_lo;
extern void const* __stack_hi;
}

/*
Reset handler and basic initialization for the RP2350.
*/

namespace sys {

constexpr u32 const kInitialStackSize {256};

[[gnu::used]] [[gnu::retain]] [[noreturn]] void reset();

} // namespace sys
