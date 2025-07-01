#include "rp2350/Clocks.h"

void XOSC::init(u32 delayCycles) {
  startup.set(20, 20, 0).set(13, 0, delayCycles >> 8); // delay = 256 * N / 12M = appx 21 ms
  control.set(11, 0, 0xaa0);                           // 0xaa0 means 1-15 MHz
  control.set(23, 12, 0xfab);                          // 0xfab means enable
  dormant.set(31, 0, 0x77616b65);                      // "wake"
  while (!status.bit(31)) { _BUSY_LOOP(); }            // wait for stable
}

void PLL::init(u16 fbDiv, u8 div1, u8 div2, u8 refDiv) {
  if (div1 < div2) { init(fbDiv, div2, div1, refDiv); }
  cs.set(5, 0, refDiv);
  fbdiv.set(11, 0, fbDiv);                  // VCO will be 12MHz XOSC * N => 700 to 1600MHz
  prim.set(18, 16, div1).set(14, 12, div2); // Divide back down (larger divisor first)
  power.set(5, 0, 0);                       // turn off power-down bits
  while (!cs.bit(31)) { _BUSY_LOOP(); }     // wait for LOCK
}