#include "RP2350/VecTable.h"
#include "runtime/Panic.h"

extern "C" {
[[noreturn]] void start();
}

[[noreturn]] void Handlers::reset() {
  start();
  __builtin_unreachable();
}

void __attribute__((interrupt)) Handlers::nmi() {}
void __attribute__((interrupt)) Handlers::sysTick() {}

void __attribute__((interrupt)) Handlers::memManage() {}
void __attribute__((interrupt)) Handlers::svCall() {}
void __attribute__((interrupt)) Handlers::pendSV() {}

void __attribute__((interrupt)) Handlers::dbgMon() {}

void __attribute__((interrupt)) Handlers::hardFault() { __panic("hardFault", "", "", 0, 0, 0); }
void __attribute__((interrupt)) Handlers::busFault() { __panic("busFault", "", "", 0, 0, 0); }
void __attribute__((interrupt)) Handlers::usageFault() { __panic("usageFault", "", "", 0, 0, 0); }

unsigned volatile FOO;

[[gnu::noinline]] void Handlers::unknown(u8 i) { FOO = i; }

[[gnu::noinline]] void Handlers::irq(u8 i) { FOO = i; }

using H = Handlers;

constexpr u32 kStackTop = 0x20000400;

// clang-format off
[[gnu::section(".vec_table")]] [[gnu::used]] [[gnu::retain]]
constexpr struct {
  u32 initialSP {kStackTop};
  void (*initialPC)() {H::reset};
  H::Handler intHandlers[14] {
      /* (sp) */      /* (pc) */      H::nmi,         H::hardFault,
      H::memManage,   H::busFault,    H::usageFault,  H::unknown<7>,
      H::unknown<8>,  H::unknown<9>,  H::unknown<10>, H::svCall,
      H::dbgMon,      H::unknown<13>, H::pendSV,      H::sysTick,
  };
  H::Handler irqHandlers[48] {
      H::irq<0>,      H::irq<1>,      H::irq<2>,      H::irq<3>,
      H::irq<4>,      H::irq<5>,      H::irq<6>,      H::irq<7>,
      H::irq<8>,      H::irq<9>,      H::irq<10>,     H::irq<11>,
      H::irq<12>,     H::irq<13>,     H::irq<14>,     H::irq<15>,
      H::irq<16>,     H::irq<17>,     H::irq<18>,     H::irq<19>,
      H::irq<20>,     H::irq<21>,     H::irq<22>,     H::irq<23>,
      H::irq<24>,     H::irq<25>,     H::irq<26>,     H::irq<27>,
      H::irq<28>,     H::irq<29>,     H::irq<30>,     H::irq<31>,
      H::irq<32>,     H::irq<33>,     H::irq<34>,     H::irq<35>,
      H::irq<36>,     H::irq<37>,     H::irq<38>,     H::irq<39>,
      H::irq<40>,     H::irq<41>,     H::irq<42>,     H::irq<43>,
      H::irq<44>,     H::irq<45>,     H::irq<46>,     H::irq<47>,
  };
} vecTable;
// clang-format on
