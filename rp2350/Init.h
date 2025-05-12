#pragma once

#include "RP2350/Common.h"

/*
Reset handler and basic initialization for the RP2350.
*/

extern "C" {
extern void _main();
}

namespace sys {

constexpr u32 const kInitialStackSize {256};

[[gnu::used]] [[gnu::retain]] [[noreturn]] void reset();

} // namespace sys
