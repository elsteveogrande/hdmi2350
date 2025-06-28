#pragma once

#include "rp2350/Common.h"

struct M33 {
  constexpr static u32 kPPBBase = 0xe0000000;

  Reg32 nvicISER0 {kPPBBase + 0xe100}; // set enable IRQ (bits 0..31)
  Reg32 nvicICER0 {kPPBBase + 0xe180}; // clear enable IRQ (bits 0..31)
  Reg32 nvicISPR0 {kPPBBase + 0xe200}; // set pending state of IRQ (bits 0..31)
  Reg32 nvicICPR0 {kPPBBase + 0xe280}; // clear pending state of IRQ (bits 0..31)
  Reg32 ccr {kPPBBase + 0xed14};       // Config and Control reg
};
