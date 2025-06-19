#include "rp2350/Common.h"

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

  Reg32 cpuID() { return {kBase + 0x000}; }
  Reg32 gpioIn() { return {kBase + 0x004}; }

  Reg32 gpioOut() { return {kBase + 0x010}; }
  Reg32 gpioOutSet() { return {kBase + 0x018}; }
  Reg32 gpioOutClr() { return {kBase + 0x020}; }
  Reg32 gpioOutXor() { return {kBase + 0x028}; }

  Reg32 gpioOutEnb() { return {kBase + 0x030}; }
  Reg32 gpioOutEnbSet() { return {kBase + 0x038}; }
  Reg32 gpioOutEnbClr() { return {kBase + 0x040}; }
  Reg32 gpioOutEnbXor() { return {kBase + 0x048}; }

  // TODO:
  // fifo(Status,Write)
  // spinlockState
  // 0x080 - 0x0bc , INTERP0
  // 0x0c0 - 0x0fc , INTERP1
  // 0x100 - 0x17c , SPINLOCKn
  // lots more
};
