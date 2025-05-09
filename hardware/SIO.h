#include "../base/Reg.h"
#include "../base/Types.h"

/*
Section 9.8, Processor GPIO Controls (SIO)
https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf
*/

struct SIO {
  // 3.1.11. List of Registers
  constexpr static u32 kBase {0xd0000000}; // SIO_BASE

  constexpr static R32 cpuID {kBase + 0x000};
  constexpr static R32 gpioIn {kBase + 0x004};
  constexpr static R32 gpioHiIn {kBase + 0x008};
  constexpr static R32 gpioOut {kBase + 0x010};
  constexpr static R32 gpioHiOut {kBase + 0x014};
  constexpr static R32 gpioOutSet {kBase + 0x018};
  constexpr static R32 gpioHiOutSet {kBase + 0x01c};
  constexpr static R32 gpioOutClr {kBase + 0x020};
  constexpr static R32 gpioHiOutClr {kBase + 0x024};
  constexpr static R32 gpioOutXor {kBase + 0x028};
  constexpr static R32 gpioHiOutXor {kBase + 0x02c};
  constexpr static R32 gpioOutEnb {kBase + 0x030};
  constexpr static R32 gpioHiOutEnb {kBase + 0x034};
  constexpr static R32 gpioOutEnbSet {kBase + 0x038};
  constexpr static R32 gpioHiOutEnbSet {kBase + 0x03c};
  constexpr static R32 gpioOutEnbClr {kBase + 0x040};
  constexpr static R32 gpioHiOutEnbClr {kBase + 0x044};
  constexpr static R32 gpioOutEnbXor {kBase + 0x048};
  constexpr static R32 gpioHiOutEnbXor {kBase + 0x04c};
  // TODO:
  // fifo(Status,Write)
  // spinlockState
  // 0x080 - 0x0bc , INTERP0
  // 0x0c0 - 0x0fc , INTERP1
  // 0x100 - 0x17c , SPINLOCKn
  // lots more
};
