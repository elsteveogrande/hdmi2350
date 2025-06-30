#pragma once

#include "rp2350/Common.h"

// 3.7. Cortex-M33 Processor

struct M33 {
  constexpr static u32 kPPBBase = 0xe0000000;

  struct NVIC {
    Reg32 ser0 {kPPBBase + 0xe100}; // Interrupt (0..31) Set Enable Registers
    Reg32 cer0 {kPPBBase + 0xe180}; // Interrupt (0..31) Clear Enable Registers
    Reg32 spr0 {kPPBBase + 0xe200}; // Interrupt (0..31) Set Pending Registers
    Reg32 cpr0 {kPPBBase + 0xe280}; // Interrupt (0..31) Clear Pending Registers

    void enableIRQ(u8 irq) { ser0.bit(irq, true); }
    void disableIRQ(u8 irq) { cer0.bit(irq, true); }
    void triggerIRQ(u8 irq) { spr0.bit(irq, true); }
  };
  NVIC nvic;

  struct ACTLR : Reg32 {};
  ACTLR ACTLR {0xE000E008}; // Auxiliary Control Register - Cortex-M33

  struct CPUID : Reg32 {};
  CPUID CPUID {0xE000ED00}; // CPUID Base Register - ARMv8M

  struct ICSRStructs {
    struct Fields {
      bool  pendingNMI(this auto const& self) { return self.bit(31); }
      auto& pendingNMIClear(this auto const& self, bool v) { return self.bit(31, v); }

      bool  pendingSV(this auto const& self) { return self.bit(30); }
      auto& pendingSVClear(this auto const& self, bool v) { return self.bit(30, v); }

      bool  pendingSysTick(this auto const& self) { return self.bit(28); }
      auto& pendingSysTickClear(this auto const& self, bool v) { return self.bit(28, v); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { explicit R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { explicit U(auto* reg) : Update(reg) {} };
    // clang-format on
  };
  using ICSR = ICSRStructs::R;
  ICSR icsr {0xE000ED04}; // M33 Interrupt Control and State Register

  struct VTOR : Reg32 {};
  VTOR VTOR {0xE000ED08}; // Vector Table Offset Register

  struct AIRCR : Reg32 {};
  AIRCR AIRCR {0xE000ED0C}; // Application Interrupt and Reset Control Register - ARMv8

  struct SCR : Reg32 {};
  SCR SCR {0xE000ED10}; // System Control Register - Cortex-M33

  struct CCRStructs {
    struct Fields {
      auto& div0Trap(this auto const& self, bool v) { return self.bit(4, v); }
      auto& unalignedTrap(this auto const& self, bool v) { return self.bit(3, v); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { explicit R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { explicit U(auto* reg) : Update(reg) {} };
    // clang-format on
  };
  using CCR = CCRStructs::R;
  CCR ccr {0xE000ED14}; // Configuration and Control Register

  struct SHPR1 : Reg32 {};
  SHPR1 SHPR1 {0xE000ED18}; // System Handler Priority Register 1 -ARMv8M

  struct SHPR2 : Reg32 {};
  SHPR2 SHPR2 {0xE000ED1C}; // System Handler Priority Register 2

  struct SHPR3 : Reg32 {};
  SHPR3 SHPR3 {0xE000ED20}; // System Handler Priority Register 3

  struct SHCSR : Reg32 {};
  SHCSR SHCSR {0xE000ED24}; // System Handler Control and State Register - ARMv8M

  struct CFSR : Reg32 {};
  CFSR CFSR {0xE000ED28}; // M33 Configurable Fault Status Register
  // MMFSR2 MMFSR2 {0xE000ED28}; // M33 MemManage Fault Status Register
  // BFSR2 BFSR2 {0xE000ED29}; // M33 BusFault Status Register
  // UFSR2 UFSR2 {0xE000ED2A}; // UsageFault Status Register

  struct HFSR : Reg32 {};
  HFSR HFSR {0xE000ED2C}; // M33 HardFault Status Register

  struct MMFAR : Reg32 {};
  MMFAR MMFAR {0xE000ED34}; // MemManage Fault Address Register

  struct BFAR : Reg32 {};
  BFAR BFAR {0xE000ED38}; // BusFault Address Register

  // struct AFSR : Reg32 {};
  // AFSR AFSR {0xE000ED3C}; // Auxiliary Fault Status Register not implemented

  struct CPACR : Reg32 {};
  CPACR CPACR {0xE000ED88}; // Coprocessor Access Control Register - ARMv8M

  struct NSACR : Reg32 {};
  NSACR NSACR {0xE000ED8C}; // Non-secure Access Control Register
};
