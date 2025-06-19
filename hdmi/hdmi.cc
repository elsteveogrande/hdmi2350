#include "rp2350/Common.h"
#include "rp2350/M33.h"
#include "rp2350/Resets.h"
#include "rp2350/SIO.h"
#include "rp2350/VecTable.h"

// extern "C" {
// // Defined in `fb.rs`:
// void colorbars();
// }

// /** These are declared extern here but the addresses are actually defined in the linker script.
// */ extern void* __fb_base; extern void* __tx_line0; extern void* __tx_line1;

// /**
// Video mode definition; assume CVT-RB.
// https://en.wikipedia.org/wiki/Coordinated_Video_Timings#Reduced_blanking
// https://tomverbeure.github.io/video_timings_calculator
// */
// struct HDMIMode {
//   u16 width;
//   u16 height;
//   u8  hFront;
//   u8  hSync;
//   u8  hBack;
//   u8  vFront;
//   u8  vSync;
//   u8  vBack;
//   u32 pixelHz;
// };

// struct FBMode {
//   u16 width;
//   u16 height;
//   u8  xRepeat;
//   u8  yRepeat;
// };

// /** An HSTX differential pair. */
// struct TXPair {
//   u8 pos;
//   u8 neg;
// };

// /** DVI 3.2.1. Channel Mapping */
// struct HDMIChannels {
//   /** Channels 0, 1, 2, and Clock, respectively */
//   TXPair pairs[4];
// };

// namespace {
// __attribute__((noinline)) void usec(unsigned us) {
//   for (unsigned j = 0; j < us; j++) {
//     for (unsigned i = 0; i < 3; i++) {
//       asm volatile("");
//       ArmInsns::nop();
//       asm volatile("");
//     }
//   }
// }
// __attribute__((noinline)) void msec(unsigned ms) {
//   for (unsigned j = 0; j < ms; j++) {
//     asm volatile("");
//     usec(1000);
//     asm volatile("");
//   }
// }

// struct SPIBanger {
//   SIO sio;

//   constexpr static unsigned kSDI    = 16; //
//   constexpr static unsigned kSDO    = 19; //
//   constexpr static unsigned kSCLK   = 18; // strobes high
//   constexpr static unsigned knCS    = 17; // low to select
//   constexpr static unsigned kLight  = 20; // high to turn on
//   constexpr static unsigned kDC     = 21; // low indicates command
//   constexpr static unsigned knReset = 22; // active low

//   void bit(bool b) {
//     sio.gpioOut.bit(kSDO, b);
//     sio.gpioOut.bit(kSCLK, 1);
//     sio.gpioOut.bit(kSCLK, 0);
//   }

//   void tx(u8 v) {
//     bit(!!(v & 0x80));
//     bit(!!(v & 0x40));
//     bit(!!(v & 0x20));
//     bit(!!(v & 0x10));
//     bit(!!(v & 0x08));
//     bit(!!(v & 0x04));
//     bit(!!(v & 0x02));
//     bit(!!(v & 0x01));
//   }

//   void cmd(u8 v) {
//     sio.gpioOut.bit(kDC, 0);
//     tx(v);
//   }

//   void data(u8 v) {
//     sio.gpioOut.bit(kDC, 1);
//     tx(v);
//   }

//   explicit SPIBanger(SIO _sio) : sio(_sio) {
//     initGPIOOut(knReset);
//     initGPIOOut(kDC);
//     initGPIOOut(kLight);
//     initGPIOOut(knCS);
//     initGPIOOut(kSCLK);
//     initGPIOOut(kSDO);
//     initGPIOIn(kSDI);
//     sio.gpioOut.bit(kSCLK, 0);
//     sio.gpioOut.bit(knCS, 0);
//     sio.gpioOut.bit(kLight, 1);
//     msec(300);
//   }
// };

// void spiStuff() {
//   SIO       sio;
//   SPIBanger spi {sio};

//   while (true) {
//     // sio.gpioOut.bit(SPIBanger::knCS, 0);
//     spi.cmd(0x53);
//     spi.data(0x00);
//     // sio.gpioOut.bit(SPIBanger::knCS, 1);
//     sio.gpioOutXor.bit(kPicoLED, 1);
//   }
// }

// } // namespace

// /*
// HDMI implementation.
// Specs for DVI (which also apply to HDMI)
// https://www.cs.unc.edu/Research/stc/FAQs/Video/dvi_spec-V1_0.pdf
// */
// struct HDMI {
//   constexpr static HDMIMode kHDMIMode {
//       .width   = 960,
//       .height  = 496,
//       .hFront  = 48,
//       .hSync   = 32,
//       .hBack   = 80,
//       .vFront  = 3,
//       .vSync   = 7,
//       .vBack   = 6,
//       .pixelHz = (960 * 496 * 60),
//   };

//   constexpr static FBMode kFBMode {
//       .width   = 400,
//       .height  = 240,
//       .xRepeat = 2,
//       .yRepeat = 2,
//   };

//   constexpr static HDMIChannels channels {{
//       {12, 13}, // CH0 (blue, VSYNC, HSYNC)
//       {14, 15}, // CH1 (green, CTL0, CTL1)
//       {16, 17}, // CH2 (red, CTL2, CTL3)
//       {18, 19}, // Clock
//   }};

//   [[gnu::used]] [[gnu::retain]] [[noreturn]] void run() {
//     SIO sio;

//     // Set up HSTX pins
//     for (u8 i = 12; i < 20; i++) { initHSTX(i); }

//     // TODO: p1203 HSTX pin configs

//     // Set up HSTX IRQ

//     // Set up HSTX DMA

//     // Set up framebuffer: init with simply a colorbar pattern
//     colorbars();

//     // Set up line buffers

//     // Start DMA out on line 0

//     // SPIBanger spi {sio};

//     while (true) {
//       sio.gpioOut.bit(kPicoLED, 1);
//       msec(100);
//       sio.gpioOut.bit(kPicoLED, 0);
//       msec(900);
//     }

//     // DMA takes over from here and all the HDMI activity is IRQ-driven.
//     // Nothing else for `run` to do; just blink the LED to show that we're up and running.
//     // unsigned line = 0;
//     // while (true) {
//     //   sio.gpioOut.bit(kPicoLED, !((line >> 15) & 1));
//     //   ++line;
//     //   _BUSY_LOOP();
//     // }
//   }

//   u32 frame_ {};
//   u16 line_ {};
// };

// struct USB {
//   void init() {
//     Resets resets;
//     resets.reset.resetUSBCTRL(false);
//     while (!resets.resetDone.resetDoneUSBCTRL()) {}
//   }
// };

/** Section 8.5 */
struct Ticks {
  struct ControlStructs {
    struct Fields {
      bool  running(this const auto& self) { return self.bit(1); }
      bool  enable(this const auto& self) { return self.bit(0); }
      auto& enable(this const auto& self, bool v) { return self.bit(0, v); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct CyclesStructs {
    struct Fields {
      u8    count(this const auto& self) { return self.get(8, 0); }
      auto& count(this const auto& self, u8 v) { return self.set(8, 0, v); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct CountStructs {
    struct Fields {
      u8 countdown(this const auto& self) { return self.get(8, 0); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  using Control = ControlStructs::R;
  using Cycles  = CyclesStructs::R;
  using Count   = CountStructs::R;

  constexpr static u32 kBase = 0x40108000;

  Control proc0Control() { return {kBase + 0x00}; }
  Cycles  proc0Cycles() { return {kBase + 0x04}; }
  Count   proc0Count() { return {kBase + 0x08}; }
  Control proc1Control() { return {kBase + 0x0c}; }
  Cycles  proc1Cycles() { return {kBase + 0x10}; }
  Count   proc1Count() { return {kBase + 0x14}; }
  Control timer0Control() { return {kBase + 0x18}; }
  Cycles  timer0Cycles() { return {kBase + 0x1c}; }
  Count   timer0Count() { return {kBase + 0x20}; }
  Control timer1Control() { return {kBase + 0x24}; }
  Cycles  timer1Cycles() { return {kBase + 0x28}; }
  Count   timer1Count() { return {kBase + 0x2c}; }
  Control watchdogControl() { return {kBase + 0x30}; }
  Cycles  watchdogCycles() { return {kBase + 0x34}; }
  Count   watchdogCount() { return {kBase + 0x38}; }
  Control riscvControl() { return {kBase + 0x3c}; }
  Cycles  riscvCycles() { return {kBase + 0x40}; }
  Count   riscvCount() { return {kBase + 0x44}; }
};

struct Timer {
  u32   base_;
  Reg32 timeLR() { return {base_ + 0x0c}; }
  Reg32 alarm0() { return {base_ + 0x10}; }
  Reg32 intEnable() { return {base_ + 0x40}; }
};

struct Timers {
  constexpr static u32 kTimer0Base {0x400b0000};
  constexpr static u32 kTimer1Base {0x400b8000};

  Timer timer0() { return {Timers::kTimer0Base}; }
  Timer timer1() { return {Timers::kTimer1Base}; }
};

u32 millis;

void timer0Callback() {

  ++millis;

  M33    m33;
  Timers timers;
  auto   t = timers.timer0();

  // Reschedule alarm
  t.alarm0().set(1000 + t.timeLR().val());

  // Reenable firing of alarm
  // p83: Interrupt 0 is TIMER_IRQ_0 by convention
  m33.nvicICPR0.bit(0, true); // Clear IRQ flag
  m33.nvicISER0.bit(0, true); // [Re]enable IRQ
}

struct Global {
  static void initTicks() {
    // p569: SDK expects nominal 1uS system ticks, as does Arm internals.
    // Although we don't use the SDK we'll assume 1uS everywhere as well.
    Ticks  ticks;
    Resets resets;

    resets.reset.resetTIMER0(false);
    while (!resets.resetDone.resetDoneTIMER0()) { _BUSY_LOOP(); }

    ticks.proc0Control().enable(0); // disable for config
    ticks.proc0Cycles().count(12);  // 12 clocks @ 12 MHz xtal -> 1 µs
    ticks.proc0Control().enable(1);
    while (!ticks.proc0Control().running()) { _BUSY_LOOP(); }

    ticks.timer0Control().enable(0); // disable for config
    ticks.timer0Cycles().count(12);  // 12 clocks @ 12 MHz xtal -> 1 µs
    ticks.timer0Control().enable(1);
    while (!ticks.timer0Control().running()) { _BUSY_LOOP(); }
  }

  static void initMSecTimer() {
    millis = 0;

    Handlers::handlers[0] = timer0Callback;
    timer0Callback(); // schedules alarm and (re-)enables
    Timers().timer0().intEnable().bit(0, true);
  }

  static void init() {
    initGPIOOut(kPicoLED);
    // Resets {}.reset.resetIOBANK0(false).resetPADSBANK0(false);
    // initTicks();
    // initMSecTimer();
    // __enable_irq();
  }
};

extern "C" {
[[gnu::used]] [[gnu::retain]] [[noreturn]] void start() {
  Global::init();

  SIO  sio;
  bool led {true};
  while (true) {
    sio.gpioOut().bit(kPicoLED, led);
    // led = !led;
    // sio.gpioOut().bit(kPicoLED, (millis >> 5) & 0x01);
    _BUSY_LOOP();
  }

  // USB usb;
  // usb.init();

  // HDMI hdmi;
  // hdmi.run();
}
}
