#include "RP2350/Common.h"

/*
Section 9.8, Processor GPIO Controls (SIO)
https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf

Note that for simplicity this only supports Bank0.
We'll assume Bank1 is not dealt with; the pins in Bank1
will then continue to be reserved only for QSPI.
*/

struct SIO {
  // 3.1.11. List of Registers
  constexpr static u32 kBase {0xd0000000}; // SIO_BASE

  constexpr static Reg cpuID {kBase + 0x000};
  constexpr static Reg gpioIn {kBase + 0x004};

  constexpr static Reg gpioOut {kBase + 0x010};
  constexpr static Reg gpioOutSet {kBase + 0x018};
  constexpr static Reg gpioOutClr {kBase + 0x020};
  constexpr static Reg gpioOutXor {kBase + 0x028};

  constexpr static Reg gpioOutEnb {kBase + 0x030};
  constexpr static Reg gpioOutEnbSet {kBase + 0x038};
  constexpr static Reg gpioOutEnbClr {kBase + 0x040};
  constexpr static Reg gpioOutEnbXor {kBase + 0x048};

  // TODO:
  // fifo(Status,Write)
  // spinlockState
  // 0x080 - 0x0bc , INTERP0
  // 0x0c0 - 0x0fc , INTERP1
  // 0x100 - 0x17c , SPINLOCKn
  // lots more
};
