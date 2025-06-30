#include "rp2350/Common.h"
#include "rp2350/M33.h"
#include "rp2350/Resets.h"
#include "rp2350/Timers.h"

u32 millis {0};

void enableIRQs() {
  M33 m33;
  // p83: Interrupt 0 is TIMER_IRQ_0 by convention
  m33.nvic.cpr0.bit(16, true); // Clear IRQ flag
  m33.nvic.ser0.bit(16, true); // [Re]enable IRQ
}

void timer0Rearm() {
  auto t0 = Timers::timer0();
  t0.alarm0().set(1000 + t0.timeLR().val());
  enableIRQs();
}

void timer0Callback() {
  ++millis;
  Reg32 {0xeeeeeeee}.set(0xf0ccface);
  timer0Rearm();
}

void milliTimerInit() {
  Resets resets;
  resets.cycleTIMER0();
  auto t0 = Timers::timer0();
  t0.intEnable();
  timer0Rearm();
}

[[gnu::used]] [[gnu::retain]] void runtimeInit() { milliTimerInit(); }
