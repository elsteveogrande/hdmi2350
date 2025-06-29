#pragma once

#include "rp2350/Common.h"

/*
Section 7.5, Subsystem Resets
https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf
*/

struct Resets {

  // clang-format off

  struct ResetStructs {
    struct Fields {
      bool resetADC       (this const auto& self) { return self.bit(0); }
      bool resetBUSCTRL   (this const auto& self) { return self.bit(1); }
      bool resetDMA       (this const auto& self) { return self.bit(2); }
      bool resetHSTX      (this const auto& self) { return self.bit(3); }
      bool resetI2C0      (this const auto& self) { return self.bit(4); }
      bool resetI2C1      (this const auto& self) { return self.bit(5); }
      bool resetIOBANK0   (this const auto& self) { return self.bit(6); }
      bool resetIOQSPI    (this const auto& self) { return self.bit(7); }
      bool resetJTAG      (this const auto& self) { return self.bit(8); }
      bool resetPADSBANK0 (this const auto& self) { return self.bit(9); }
      bool resetPADSQSPI  (this const auto& self) { return self.bit(10); }
      bool resetPIO0      (this const auto& self) { return self.bit(11); }
      bool resetPIO1      (this const auto& self) { return self.bit(12); }
      bool resetPIO2      (this const auto& self) { return self.bit(13); }
      bool resetPLLSYS    (this const auto& self) { return self.bit(14); }
      bool resetPLLUSB    (this const auto& self) { return self.bit(15); }
      bool resetPWM       (this const auto& self) { return self.bit(16); }
      bool resetSHA256    (this const auto& self) { return self.bit(17); }
      bool resetSPI0      (this const auto& self) { return self.bit(18); }
      bool resetSPI1      (this const auto& self) { return self.bit(19); }
      bool resetSYSCFG    (this const auto& self) { return self.bit(20); }
      bool resetSYSINFO   (this const auto& self) { return self.bit(21); }
      bool resetTBMAN     (this const auto& self) { return self.bit(22); }
      bool resetTIMER0    (this const auto& self) { return self.bit(23); }
      bool resetTIMER1    (this const auto& self) { return self.bit(24); }
      bool resetTRNG      (this const auto& self) { return self.bit(25); }
      bool resetUART0     (this const auto& self) { return self.bit(26); }
      bool resetUART1     (this const auto& self) { return self.bit(27); }
      bool resetUSBCTRL   (this const auto& self) { return self.bit(28); }

      auto& resetADC       (this const auto& self, bool v) { return self.bit(0, v); }
      auto& resetBUSCTRL   (this const auto& self, bool v) { return self.bit(1, v); }
      auto& resetDMA       (this const auto& self, bool v) { return self.bit(2, v); }
      auto& resetHSTX      (this const auto& self, bool v) { return self.bit(3, v); }
      auto& resetI2C0      (this const auto& self, bool v) { return self.bit(4, v); }
      auto& resetI2C1      (this const auto& self, bool v) { return self.bit(5, v); }
      auto& resetIOBANK0   (this const auto& self, bool v) { return self.bit(6, v); }
      auto& resetIOQSPI    (this const auto& self, bool v) { return self.bit(7, v); }
      auto& resetJTAG      (this const auto& self, bool v) { return self.bit(8, v); }
      auto& resetPADSBANK0 (this const auto& self, bool v) { return self.bit(9, v); }
      auto& resetPADSQSPI  (this const auto& self, bool v) { return self.bit(10, v); }
      auto& resetPIO0      (this const auto& self, bool v) { return self.bit(11, v); }
      auto& resetPIO1      (this const auto& self, bool v) { return self.bit(12, v); }
      auto& resetPIO2      (this const auto& self, bool v) { return self.bit(13, v); }
      auto& resetPLLSYS    (this const auto& self, bool v) { return self.bit(14, v); }
      auto& resetPLLUSB    (this const auto& self, bool v) { return self.bit(15, v); }
      auto& resetPWM       (this const auto& self, bool v) { return self.bit(16, v); }
      auto& resetSHA256    (this const auto& self, bool v) { return self.bit(17, v); }
      auto& resetSPI0      (this const auto& self, bool v) { return self.bit(18, v); }
      auto& resetSPI1      (this const auto& self, bool v) { return self.bit(19, v); }
      auto& resetSYSCFG    (this const auto& self, bool v) { return self.bit(20, v); }
      auto& resetSYSINFO   (this const auto& self, bool v) { return self.bit(21, v); }
      auto& resetTBMAN     (this const auto& self, bool v) { return self.bit(22, v); }
      auto& resetTIMER0    (this const auto& self, bool v) { return self.bit(23, v); }
      auto& resetTIMER1    (this const auto& self, bool v) { return self.bit(24, v); }
      auto& resetTRNG      (this const auto& self, bool v) { return self.bit(25, v); }
      auto& resetUART0     (this const auto& self, bool v) { return self.bit(26, v); }
      auto& resetUART1     (this const auto& self, bool v) { return self.bit(27, v); }
      auto& resetUSBCTRL   (this const auto& self, bool v) { return self.bit(28, v); }
    };
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
  };

  struct WDSelStructs {
    struct Fields {
      bool selADC       (this const auto& self) { return self.bit(0); }
      bool selBUSCTRL   (this const auto& self) { return self.bit(1); }
      bool selDMA       (this const auto& self) { return self.bit(2); }
      bool selHSTX      (this const auto& self) { return self.bit(3); }
      bool selI2C0      (this const auto& self) { return self.bit(4); }
      bool selI2C1      (this const auto& self) { return self.bit(5); }
      bool selIOBANK0   (this const auto& self) { return self.bit(6); }
      bool selIOQSPI    (this const auto& self) { return self.bit(7); }
      bool selJTAG      (this const auto& self) { return self.bit(8); }
      bool selPADSBANK0 (this const auto& self) { return self.bit(9); }
      bool selPADSQSPI  (this const auto& self) { return self.bit(10); }
      bool selPIO0      (this const auto& self) { return self.bit(11); }
      bool selPIO1      (this const auto& self) { return self.bit(12); }
      bool selPIO2      (this const auto& self) { return self.bit(13); }
      bool selPLLSYS    (this const auto& self) { return self.bit(14); }
      bool selPLLUSB    (this const auto& self) { return self.bit(15); }
      bool selPWM       (this const auto& self) { return self.bit(16); }
      bool selSHA256    (this const auto& self) { return self.bit(17); }
      bool selSPI0      (this const auto& self) { return self.bit(18); }
      bool selSPI1      (this const auto& self) { return self.bit(19); }
      bool selSYSCFG    (this const auto& self) { return self.bit(20); }
      bool selSYSINFO   (this const auto& self) { return self.bit(21); }
      bool selTBMAN     (this const auto& self) { return self.bit(22); }
      bool selTIMER0    (this const auto& self) { return self.bit(23); }
      bool selTIMER1    (this const auto& self) { return self.bit(24); }
      bool selTRNG      (this const auto& self) { return self.bit(25); }
      bool selUART0     (this const auto& self) { return self.bit(26); }
      bool selUART1     (this const auto& self) { return self.bit(27); }
      bool selUSBCTRL   (this const auto& self) { return self.bit(28); }

      auto& selADC       (this const auto& self, bool v) { return self.bit(0, v); }
      auto& selBUSCTRL   (this const auto& self, bool v) { return self.bit(1, v); }
      auto& selDMA       (this const auto& self, bool v) { return self.bit(2, v); }
      auto& selHSTX      (this const auto& self, bool v) { return self.bit(3, v); }
      auto& selI2C0      (this const auto& self, bool v) { return self.bit(4, v); }
      auto& selI2C1      (this const auto& self, bool v) { return self.bit(5, v); }
      auto& selIOBANK0   (this const auto& self, bool v) { return self.bit(6, v); }
      auto& selIOQSPI    (this const auto& self, bool v) { return self.bit(7, v); }
      auto& selJTAG      (this const auto& self, bool v) { return self.bit(8, v); }
      auto& selPADSBANK0 (this const auto& self, bool v) { return self.bit(9, v); }
      auto& selPADSQSPI  (this const auto& self, bool v) { return self.bit(10, v); }
      auto& selPIO0      (this const auto& self, bool v) { return self.bit(11, v); }
      auto& selPIO1      (this const auto& self, bool v) { return self.bit(12, v); }
      auto& selPIO2      (this const auto& self, bool v) { return self.bit(13, v); }
      auto& selPLLSYS    (this const auto& self, bool v) { return self.bit(14, v); }
      auto& selPLLUSB    (this const auto& self, bool v) { return self.bit(15, v); }
      auto& selPWM       (this const auto& self, bool v) { return self.bit(16, v); }
      auto& selSHA256    (this const auto& self, bool v) { return self.bit(17, v); }
      auto& selSPI0      (this const auto& self, bool v) { return self.bit(18, v); }
      auto& selSPI1      (this const auto& self, bool v) { return self.bit(19, v); }
      auto& selSYSCFG    (this const auto& self, bool v) { return self.bit(20, v); }
      auto& selSYSINFO   (this const auto& self, bool v) { return self.bit(21, v); }
      auto& selTBMAN     (this const auto& self, bool v) { return self.bit(22, v); }
      auto& selTIMER0    (this const auto& self, bool v) { return self.bit(23, v); }
      auto& selTIMER1    (this const auto& self, bool v) { return self.bit(24, v); }
      auto& selTRNG      (this const auto& self, bool v) { return self.bit(25, v); }
      auto& selUART0     (this const auto& self, bool v) { return self.bit(26, v); }
      auto& selUART1     (this const auto& self, bool v) { return self.bit(27, v); }
      auto& selUSBCTRL   (this const auto& self, bool v) { return self.bit(28, v); }
    };
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
  };

  struct ResetDoneStructs {
    struct Fields {
      bool resetDoneADC       (this const auto& self) { return self.bit(0); }
      bool resetDoneBUSCTRL   (this const auto& self) { return self.bit(1); }
      bool resetDoneDMA       (this const auto& self) { return self.bit(2); }
      bool resetDoneHSTX      (this const auto& self) { return self.bit(3); }
      bool resetDoneI2C0      (this const auto& self) { return self.bit(4); }
      bool resetDoneI2C1      (this const auto& self) { return self.bit(5); }
      bool resetDoneIOBANK0   (this const auto& self) { return self.bit(6); }
      bool resetDoneIOQSPI    (this const auto& self) { return self.bit(7); }
      bool resetDoneJTAG      (this const auto& self) { return self.bit(8); }
      bool resetDonePADSBANK0 (this const auto& self) { return self.bit(9); }
      bool resetDonePADSQSPI  (this const auto& self) { return self.bit(10); }
      bool resetDonePIO0      (this const auto& self) { return self.bit(11); }
      bool resetDonePIO1      (this const auto& self) { return self.bit(12); }
      bool resetDonePIO2      (this const auto& self) { return self.bit(13); }
      bool resetDonePLLSYS    (this const auto& self) { return self.bit(14); }
      bool resetDonePLLUSB    (this const auto& self) { return self.bit(15); }
      bool resetDonePWM       (this const auto& self) { return self.bit(16); }
      bool resetDoneSHA256    (this const auto& self) { return self.bit(17); }
      bool resetDoneSPI0      (this const auto& self) { return self.bit(18); }
      bool resetDoneSPI1      (this const auto& self) { return self.bit(19); }
      bool resetDoneSYSCFG    (this const auto& self) { return self.bit(20); }
      bool resetDoneSYSINFO   (this const auto& self) { return self.bit(21); }
      bool resetDoneTBMAN     (this const auto& self) { return self.bit(22); }
      bool resetDoneTIMER0    (this const auto& self) { return self.bit(23); }
      bool resetDoneTIMER1    (this const auto& self) { return self.bit(24); }
      bool resetDoneTRNG      (this const auto& self) { return self.bit(25); }
      bool resetDoneUART0     (this const auto& self) { return self.bit(26); }
      bool resetDoneUART1     (this const auto& self) { return self.bit(27); }
      bool resetDoneUSBCTRL   (this const auto& self) { return self.bit(28); }
    };
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
  };

  using Reset     = ResetStructs::R;
  using WDSel     = WDSelStructs::R;
  using ResetDone = ResetDoneStructs::R;

  constexpr static u32 kBase {0x40020000};
  Reset reset {kBase + 0};
  WDSel wdSel {kBase + 4};
  ResetDone resetDone {kBase + 8};

  void cycleADC      () { reset.resetADC      (true); reset.resetADC      (false); while (!resetDone.resetDoneADC      ()) {_BUSY_LOOP();}  }
  void cycleBUSCTRL  () { reset.resetBUSCTRL  (true); reset.resetBUSCTRL  (false); while (!resetDone.resetDoneBUSCTRL  ()) {_BUSY_LOOP();}  }
  void cycleDMA      () { reset.resetDMA      (true); reset.resetDMA      (false); while (!resetDone.resetDoneDMA      ()) {_BUSY_LOOP();}  }
  void cycleHSTX     () { reset.resetHSTX     (true); reset.resetHSTX     (false); while (!resetDone.resetDoneHSTX     ()) {_BUSY_LOOP();}  }
  void cycleI2C0     () { reset.resetI2C0     (true); reset.resetI2C0     (false); while (!resetDone.resetDoneI2C0     ()) {_BUSY_LOOP();}  }
  void cycleI2C1     () { reset.resetI2C1     (true); reset.resetI2C1     (false); while (!resetDone.resetDoneI2C1     ()) {_BUSY_LOOP();}  }
  void cycleIOBANK0  () { reset.resetIOBANK0  (true); reset.resetIOBANK0  (false); while (!resetDone.resetDoneIOBANK0  ()) {_BUSY_LOOP();}  }
  void cycleIOQSPI   () { reset.resetIOQSPI   (true); reset.resetIOQSPI   (false); while (!resetDone.resetDoneIOQSPI   ()) {_BUSY_LOOP();}  }
  void cycleJTAG     () { reset.resetJTAG     (true); reset.resetJTAG     (false); while (!resetDone.resetDoneJTAG     ()) {_BUSY_LOOP();}  }
  void cyclePADSBANK0() { reset.resetPADSBANK0(true); reset.resetPADSBANK0(false); while (!resetDone.resetDonePADSBANK0()) {_BUSY_LOOP();}  }
  void cyclePADSQSPI () { reset.resetPADSQSPI (true); reset.resetPADSQSPI (false); while (!resetDone.resetDonePADSQSPI ()) {_BUSY_LOOP();}   }
  void cyclePIO0     () { reset.resetPIO0     (true); reset.resetPIO0     (false); while (!resetDone.resetDonePIO0     ()) {_BUSY_LOOP();}   }
  void cyclePIO1     () { reset.resetPIO1     (true); reset.resetPIO1     (false); while (!resetDone.resetDonePIO1     ()) {_BUSY_LOOP();}   }
  void cyclePIO2     () { reset.resetPIO2     (true); reset.resetPIO2     (false); while (!resetDone.resetDonePIO2     ()) {_BUSY_LOOP();}   }
  void cyclePLLSYS   () { reset.resetPLLSYS   (true); reset.resetPLLSYS   (false); while (!resetDone.resetDonePLLSYS   ()) {_BUSY_LOOP();}   }
  void cyclePLLUSB   () { reset.resetPLLUSB   (true); reset.resetPLLUSB   (false); while (!resetDone.resetDonePLLUSB   ()) {_BUSY_LOOP();}   }
  void cyclePWM      () { reset.resetPWM      (true); reset.resetPWM      (false); while (!resetDone.resetDonePWM      ()) {_BUSY_LOOP();}   }
  void cycleSHA256   () { reset.resetSHA256   (true); reset.resetSHA256   (false); while (!resetDone.resetDoneSHA256   ()) {_BUSY_LOOP();}   }
  void cycleSPI0     () { reset.resetSPI0     (true); reset.resetSPI0     (false); while (!resetDone.resetDoneSPI0     ()) {_BUSY_LOOP();}   }
  void cycleSPI1     () { reset.resetSPI1     (true); reset.resetSPI1     (false); while (!resetDone.resetDoneSPI1     ()) {_BUSY_LOOP();}   }
  void cycleSYSCFG   () { reset.resetSYSCFG   (true); reset.resetSYSCFG   (false); while (!resetDone.resetDoneSYSCFG   ()) {_BUSY_LOOP();}   }
  void cycleSYSINFO  () { reset.resetSYSINFO  (true); reset.resetSYSINFO  (false); while (!resetDone.resetDoneSYSINFO  ()) {_BUSY_LOOP();}   }
  void cycleTBMAN    () { reset.resetTBMAN    (true); reset.resetTBMAN    (false); while (!resetDone.resetDoneTBMAN    ()) {_BUSY_LOOP();}   }
  void cycleTIMER0   () { reset.resetTIMER0   (true); reset.resetTIMER0   (false); while (!resetDone.resetDoneTIMER0   ()) {_BUSY_LOOP();}   }
  void cycleTIMER1   () { reset.resetTIMER1   (true); reset.resetTIMER1   (false); while (!resetDone.resetDoneTIMER1   ()) {_BUSY_LOOP();}   }
  void cycleTRNG     () { reset.resetTRNG     (true); reset.resetTRNG     (false); while (!resetDone.resetDoneTRNG     ()) {_BUSY_LOOP();}   }
  void cycleUART0    () { reset.resetUART0    (true); reset.resetUART0    (false); while (!resetDone.resetDoneUART0    ()) {_BUSY_LOOP();}   }
  void cycleUART1    () { reset.resetUART1    (true); reset.resetUART1    (false); while (!resetDone.resetDoneUART1    ()) {_BUSY_LOOP();}   }
  void cycleUSBCTRL  () { reset.resetUSBCTRL  (true); reset.resetUSBCTRL  (false); while (!resetDone.resetDoneUSBCTRL  ()) {_BUSY_LOOP();}   }
};
