#pragma once

/** Reset vector.  This does basic RP2350 [re-]initialization and jumps to `start`. */
[[gnu::used]] [[gnu::retain]] [[noreturn]] void reset();

extern "C" {
/** Application entry point. */
extern void start();
}
