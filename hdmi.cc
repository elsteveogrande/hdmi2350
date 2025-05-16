#include "RP2350/Common.h"
#include "RP2350/SIO.h"

extern "C" {
[[gnu::noinline]] void __aeabi_memcpy(u8* dest, const u8* src, unsigned n) {
  for (unsigned i = 0; i < n; i++) {
    u8 x = src[i];
    asm volatile("");
    dest[i] = x;
  }
}

[[gnu::noinline]] void __aeabi_memcpy4(u8* dest, const u8* src, unsigned n) {
  [[assume(!(u32(dest) & 3))]];
  [[assume(!(u32(src) & 3))]];
  __aeabi_memcpy(dest, src, n);
}
}

/** These are declared extern here but the addresses are actually defined in the linker script. */
extern void* __fb;
extern void* __line0;
extern void* __line1;

/**
Video mode definition; assume CVT-RB.
https://en.wikipedia.org/wiki/Coordinated_Video_Timings#Reduced_blanking
https://tomverbeure.github.io/video_timings_calculator
*/
struct HDMIMode {
  u16 width;
  u16 height;
  u8  hFront;
  u8  hSync;
  u8  hBack;
  u8  vFront;
  u8  vSync;
  u8  vBack;
  u32 pixelHz;
};

struct FBMode {
  u16 width;
  u16 height;
  u8  xRepeat;
  u8  yRepeat;
};

/** An HSTX differential pair. */
struct TXPair {
  u8 pos;
  u8 neg;
};

/** DVI 3.2.1. Channel Mapping */
struct HDMIChannels {
  /** Channels 0, 1, 2, and Clock, respectively */
  TXPair pairs[4];
};

/*
HDMI implementation.
Specs for DVI (which also apply to HDMI)
https://www.cs.unc.edu/Research/stc/FAQs/Video/dvi_spec-V1_0.pdf
*/
struct HDMI {
  constexpr static HDMIMode kHDMIMode {
      .width   = 960,
      .height  = 496,
      .hFront  = 48,
      .hSync   = 32,
      .hBack   = 80,
      .vFront  = 3,
      .vSync   = 7,
      .vBack   = 6,
      .pixelHz = (960 * 496 * 60),
  };

  constexpr static FBMode kFBMode {
      .width   = 400,
      .height  = 240,
      .xRepeat = 2,
      .yRepeat = 2,
  };

  constexpr static HDMIChannels channels {{
      {12, 13}, // CH0 (blue, VSYNC, HSYNC)
      {14, 15}, // CH1 (green, CTL0, CTL1)
      {16, 17}, // CH2 (red, CTL2, CTL3)
      {18, 19}, // Clock
  }};

  [[gnu::used]] [[gnu::retain]] [[noreturn]] void run() {
    // First turn on LED, to show that we at least made it to this point
    SIO sio;
    initGPIOOut(kPicoLED);
    sio.gpioOut.bit(kPicoLED, true);

    // Set up HSTX pins
    for (u8 i = 12; i < 20; i++) { initHSTX(i); }

    // Set up HSTX IRQ

    // Set up HSTX DMA

    // Set up framebuffer

    // Set up line buffers

    // Start DMA out on line 0

    // DMA takes over from here and all the HDMI activity is IRQ-driven.
    // Nothing else for `run` to do; just blink the LED to show that we're up and running.
    while (true) {
      sio.gpioOut.bit(kPicoLED, frame_ & 1);
      _BUSY_LOOP();
    }
  }

  u32 frame_ {};
  u16 line_ {};
};

extern "C" {
[[noreturn]] void start() {
  HDMI hdmi;
  hdmi.run();
}
}
