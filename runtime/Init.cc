#include "rp2350/Common.h"
#include "rp2350/M33.h"
#include "rp2350/Resets.h"
#include "rp2350/SIO.h"
#include "rp2350/Timers.h"
#include "rp2350/VecTable.h"
#include "runtime/RuntimeData.h"

// void enableIRQs() {
//   M33 m33;
//   // p83: Interrupt 0 is TIMER_IRQ_0 by convention
//   m33.nvic.cpr0.bit(0, true); // Clear IRQ flag
//   m33.nvic.ser0.bit(0, true); // [Re]enable IRQ
// }

// void timer0Rearm() {
//   auto t0 = Timers::timer0();
//   t0.alarm0().set(1000 + t0.timeLR().val());
//   enableIRQs();
// }

// void timer0Handler() {
//   ++runtimeData.millis;
//   timer0Rearm();
//   Reg32 {0xeeeeeeee}.set(0xf0ccface);
// }

// void milliTimerInit() {
//   Resets resets;
//   resets.cycleTIMER0();
//   Handlers::handlers[0] = timer0Handler;
//   Timers::timer0().intEnable();
//   timer0Rearm();
// }

[[gnu::used]] [[gnu::retain]] void runtimeInit() {
  // milliTimerInit();
  // M33 m33;
  // Insns::enableIRQs();
  // m33.nvic.triggerIRQ(0);
}
