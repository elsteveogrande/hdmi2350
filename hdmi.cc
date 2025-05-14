#include "RP2350/Common.h"
#include "RP2350/SIO.h"

/** These are declared extern here but the addresses are actually defined in the linker script. */
extern void* __fb;
extern void* __line0;
extern void* __line1;

/*
HDMI implementation.
Specs for DVI (which also apply to HDMI)
https://www.cs.unc.edu/Research/stc/FAQs/Video/dvi_spec-V1_0.pdf
*/
struct HDMI {

  /**
  Video mode definition; assume CVT-RB.
  https://en.wikipedia.org/wiki/Coordinated_Video_Timings#Reduced_blanking
  https://tomverbeure.github.io/video_timings_calculator
  */
  struct Mode {
    u16 width;
    u16 height;
    u8  xRepeat;
    u8  yRepeat;
    u8  hFront;
    u8  hSync;
    u8  hBack;
    u8  vFront;
    u8  vSync;
    u8  vBack;
    u32 pixelHz;
  };

  constexpr static Mode kModes[] {
      {.width   = 800,
       .height  = 480,
       .xRepeat = 2, // effective 400 W
       .yRepeat = 2, // effective 240 H
       .hFront  = 48,
       .hSync   = 32,
       .hBack   = 80,
       .vFront  = 3,
       .vSync   = 7,
       .vBack   = 6,
       .pixelHz = 28569600},
  };

  /* Default (and currently, only) mode */
  constexpr static auto kDefaultMode = kModes[0];

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

  HDMIChannels channels {{
      {12, 13}, // CH0 (blue, VSYNC, HSYNC)
      {14, 15}, // CH1 (green, CTL0, CTL1)
      {16, 17}, // CH2 (red, CTL2, CTL3)
      {18, 19}, // Clock
  }};

  u32 frame = 0;

  [[gnu::used]] [[gnu::retain]] [[noreturn]] void run() {
    initGPIOOut(kPicoLED);
    for (u8 i = 12; i < 20; i++) { initHSTX(i); }

    SIO sio;

    while (true) {
      sio.gpioOut.bit(kPicoLED, frame & 1);
      _BUSY_LOOP();
    }
  }
};

extern "C" {
[[noreturn]] void start() {
  HDMI hdmi;
  hdmi.run();
}
}
