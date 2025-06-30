#include "rp2350/VecTable.h"
#include "rp2350/Common.h"
#include "runtime/Panic.h"

[[gnu::aligned(16)]] void __attribute__((interrupt)) Handlers::nmi() {}
[[gnu::aligned(16)]] void __attribute__((interrupt)) Handlers::sysTick() {}
[[gnu::aligned(16)]] void __attribute__((interrupt)) Handlers::svCall() {}
[[gnu::aligned(16)]] void __attribute__((interrupt)) Handlers::pendSV() {}
[[gnu::aligned(16)]] void __attribute__((interrupt)) Handlers::dbgMon() {}

[[gnu::noinline]] [[gnu::aligned(16)]] void Handlers::unknown(u32 i) {
  // TODO: panic and report interrupt number
  (void)i;
  ArmInsns::nop();
}

[[gnu::noinline]] [[gnu::aligned(16)]] void Handlers::irqn(u8 i) {
  auto* handler = handlers[i];
  if (handler) { handler(); }
}

[[gnu::noinline]] [[gnu::aligned(16)]] void Handlers::unknown() {
  unknown(ArmInsns::ipsr() & 0x1ff);
}

[[gnu::noinline]] [[gnu::aligned(16)]] void Handlers::irq() {
  auto intn = ArmInsns::ipsr() & 0x1ff;
  if (intn >= 16 && intn < 64) {
    irqn(u8(intn - 16));
  } else {
    unknown(intn);
  }
}

using H = Handlers;

constexpr u32 kStackTop = 0x20000c00;

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
      /* 0 (initial sp) */    /* 1: (reset) */        /*2*/ H::nmi,             /*3*/ cxx::__hardFault,
      /*4*/ cxx::__memManage, /*5*/ cxx::__busFault,  /*6*/ cxx::__usageFault,  /*7*/ H::unknown,
      /*8*/ H::unknown,       /*9*/ H::unknown,       /*10*/ H::unknown,        /*11*/ H::svCall,
      /*12*/ H::dbgMon,       /*13*/ H::unknown,      /*14*/ H::pendSV,         /*15*/ H::sysTick,
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
