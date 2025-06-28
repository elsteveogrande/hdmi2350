#include "rp2350/Common.h"
#include "rp2350/GPIO.h"
#include "rp2350/HSTX.h"
#include "rp2350/M33.h"
#include "rp2350/Resets.h"
#include "rp2350/SIO.h"
#include "runtime/Panic.h"

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
//   }

//   u32 frame_ {};
//   u16 line_ {};
// };

u32 millis;

void timer0Callback() {
  // ++millis;

  // M33    m33;
  // Timers timers;
  // auto   t = timers.timer0();

  // // Reschedule alarm
  // t.alarm0().set(1000 + t.timeLR().val());

  // // Reenable firing of alarm
  // // p83: Interrupt 0 is TIMER_IRQ_0 by convention
  // m33.nvicICPR0.bit(0, true); // Clear IRQ flag
  // m33.nvicISER0.bit(0, true); // [Re]enable IRQ
}

extern "C" {

[[gnu::used]] [[gnu::retain]] [[noreturn]] void start() {

  Resets resets;
  HSTX   hstx;
  SIO    sio;

  // Init HSTX:

  // Assume system clock (SYSPLL) is 150 MHz, the default for Pico2.
  // We want to transmit at a 30 MHz pixel clock, with each TMDS bit-time
  // 1/10th of that clock (i.e. 300 MHz).

  // Control reg
  hstx.csr
      .clkDiv(5)       // output clock is 5 DDR periods
      .rotates(5)      // two bits (DDR) per bit-clock, so we'll rotate 5 times
      .rotBits(32 - 2) // left-shift two bits per complete (pos AND neg edges) input clock
      .enabled(true);

  // Set GPIOs 12..19 to HSTX instead of GPIO
  for (u8 i = 12; i < 20; i++) { initHSTX(i); }
  for (u8 i = 0; i < 8; i++) { initGPIOOut(i); }

  // Maps TMDS pairs to HSTX bits and GPIO pairs:
  //  +CLK-   +CH2-   +CH1-   +CH0-
  //   7   6   5   4   3   2   1   0  HSTX bit
  //  19  18  17  16  15  14  13  12  GPIO number
  //
  // Control signals are 30 bits and sent out like so, on each
  // transition of the HSTX clock (DDR, to rising/falling edges):
  //  xx987654321098765432109876543210  Input data, 32 bits (x=ignored)
  //    ^^        ^^        ^^          P/N DDR cycles
  // That is, each channel's bits 9 and 8 are sent per bit-clock.
  // This word is also left-rotated by two, a total of 5 times.
  // This results in the selection of these bits (in brackets):
  //  xx[98]76543210[98]76543210[98]76543210
  //  98[76]54321098[76]54321098[76]543210xx
  //  76[54]32109876[54]32109876[54]3210xx98
  //  54[32]10987654[32]10987654[32]10xx9876
  //  32[10]98765432[10]98765432[10]xx987654
  //    ^ CH2       ^ CH1       ^ CH0
  //
  // We configure the output pins to select particular bits from the data word.
  // But we can't configure 5 different sets of bits to select each time, but
  // instead, specify one selection of bits, and rotate the whole data word,
  // in order to send two bits at a time, 5 times.

  while (true) {
    resets.reset.resetHSTX(false);

    hstx.bit(7).clock(true);
    sio.gpioOut().bit(kPicoLED, 1);

    hstx.bit(5).selectP(29).selectN(28);
    hstx.bit(3).selectP(19).selectN(18);
    hstx.bit(1).selectP(9).selectN(8);

    hstx.bit(6).clock(true).invert(true);
    hstx.bit(4).selectP(29).selectN(28).invert(true);
    hstx.bit(2).selectP(19).selectN(18).invert(true);
    hstx.bit(0).selectP(9).selectN(8).invert(true);

    sio.gpioOut().set(7, 0, hstx.fifoStatus.get(15, 8));
    if (!hstx.fifoStatus.full()) { hstx.fifo.set(0xffffffff); }
    _BUSY_LOOP();
  }
}
}
