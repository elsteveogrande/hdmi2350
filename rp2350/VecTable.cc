#include "rp2350/VecTable.h"
#include "rp2350/Common.h"
#include "rp2350/ResetHandler.h"
#include "runtime/Panic.h"

void __attribute__((interrupt)) Handlers::nmi() {}
void __attribute__((interrupt)) Handlers::sysTick() {}

void __attribute__((interrupt)) Handlers::memManage() {}
void __attribute__((interrupt)) Handlers::svCall() {}
void __attribute__((interrupt)) Handlers::pendSV() {}

void __attribute__((interrupt)) Handlers::dbgMon() {}

void __attribute__((interrupt)) Handlers::hardFault() { __panic("hardFault", "", "", 0, 0, 0); }
void __attribute__((interrupt)) Handlers::busFault() { __panic("busFault", "", "", 0, 0, 0); }
void __attribute__((interrupt)) Handlers::usageFault() { __panic("usageFault", "", "", 0, 0, 0); }

[[gnu::noinline]] void Handlers::unknown(u32 i) {
  // TODO: panic and report interrupt number
  (void)i;
  ArmInsns::nop();
}

[[gnu::noinline]] void Handlers::irqn(u8 i) {
  auto* handler = handlers[i];
  if (handler) { handler(); }
}

[[gnu::noinline]] void Handlers::unknown() { unknown(ArmInsns::ipsr() & 0x1ff); }

[[gnu::noinline]] void Handlers::irq() {
  auto intn = ArmInsns::ipsr() & 0x1ff;
  if (intn >= 16 && intn < 64) {
    irqn(u8(intn - 16));
  } else {
    unknown(intn);
  }
}

using H = Handlers;

constexpr u32 kStackTop = 0x20000400;

H::Handler Handlers::handlers[64] = {nullptr};

// clang-format off
[[gnu::section(".vec_table")]] [[gnu::used]] [[gnu::retain]]
struct {
  // U32 #0
  u32 initialSP {kStackTop};
  // U32 #1
  void (*initialPC)() {H::reset};

  // U32 #2 through #15
  H::Handler intHandlers[14] {
      /* 0 (initial sp) */  /* 1: (reset) */   /*2*/ H::nmi,   /*3*/ H::hardFault,
      /*3*/ H::memManage,   /*3*/ H::busFault,    /*3*/ H::usageFault,  /*3*/ H::unknown,
      /*3*/ H::unknown,     /*3*/ H::unknown,     /*3*/ H::unknown,     /*3*/ H::svCall,
      /*3*/ H::dbgMon,      /*3*/ H::unknown,     /*3*/ H::pendSV,      /*3*/ H::sysTick,
  };

  // U32 #16 through #63
  H::Handler irqHandlers[48] {
    H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq,
    H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq,
    H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq,
    H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq, H::irq,
  };
} vecTable;
static_assert(sizeof(vecTable) == 256);

// clang-format on
