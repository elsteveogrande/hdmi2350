#pragma once

#include "rp2350/Types.h"

/*
Reset handler and basic initialization for the RP2350.
*/

namespace sys {

constexpr u32 const kInitialStackSize {256};

[[gnu::used]] [[gnu::retain]] [[noreturn]] void reset();

} // namespace sys
