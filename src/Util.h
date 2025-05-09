#pragma once

/**
Use like: `_busy_loop(__FILE__, __LINE__);`.
TODO: actually record where the busy loop happens, for diagnostics;
TODO: later, make a proper stacktrace object instead.
*/
inline void _busy_loop(char const* file = nullptr, unsigned line = 0) {
  // TODO emit a `nop` instruction, for ARM or RISCV
  (void)file;
  (void)line;
}

// clang-format off
#define _BUSY_LOOP() { do { _busy_loop(__FILE__, __LINE__); } while(false); }
// clang-format on
