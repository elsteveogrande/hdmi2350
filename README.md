# hdmi2350

**Work in progress!**

A working, but dirty and incomplete, implementation of HDMI, targeting RP2350.

These initial versions support HDMI video (no audio yet), and render
a 320x240 framebuffer at 60 Hz.  More details on that below.

# Getting started

## Dependencies

* LLVM: Clang, LLD.  Needs a new enough version which supports C++26.
* Python
* `make`

Run `make` and copy the resulting `build/hdmi2350.bin.uf2` onto your Pico2.

Note that this avoids the official `pico-sdk` project, for better or worse.
Reasons include (and I know some of these are weak)

* I prefer Clang, `clangd`, LLD, and generally the LLVM line of tools,
  over gcc.
* SDKs are great at making certain things easier for its users
  but I simply wanted to try making this thing -- literally from the
  register programming and UF2 file generation, to the part where your
  TV shows stuff -- by my own dumb, stubborn self.
* CMake, cross-project dependencies, and other things related to the
  build system is not the thing I want to deal with right now.

This is more for fun than for any serious use, so I opted to dive into
a challenging project with both feet, and force myself to learn new stuff
(and `cmake` is not _currently_ one of those things).

I also felt like this was a good opportunity to redo some things with
a modern C++ (again, forcing myself to internalize fancy C++26 features).

In general, I want an application and mini-SDK that is small, well-commented,
and easy to understand, hopefully so others can enjoy digging into the RP2350.

My thoughts about the Pico SDK: if you're building Pico code for any serious
reason, or if you have something you want to build and are comfortable with the
SDK already and you just want to get off the ground, then SDK is the way to go.
It makes certain things easy and mostly shields you from the grittiest details
like addresses, registers' bits, compiler flags, memory layout.
And it's actually supported by Raspberry Pi and a community of many professionals and hobbyists.
On the other hand, I like those weird things like memory layout, registers, and so on.
I enjoy getting into _those particular_ weeds, so I went this direction instead.

# Limitations

## RP2350-related limitations

* Currently only supports GPIOs 0 through 29
* Not attempting backwards compatibility with RP2040 (sorry)
  - The XIP and boot2 initial setup is a pain
  - Supporting two different-enough chips is difficult
* Still only targeting ARM (Cortex-M33); no RISC-V fun yet

## HDMI-related limitations

This thing is not fully HDMI compliant, even if it does tend to work with HDMI-licensed / HDMI-certified / whatever equipment.

* Mode: 640x480 @ 60p (the VGA-compatibility mode)
* Actual resolution: 320x240, aka QVGA, aka 1/27th of 1080p
  * Enough for e.g. an NES emulator (256x240)
  * I would like to improve this; at the very least I'd like to have a
    mode that gives you true square pixels in a 16:9 ASR.
* No audio yet!  But two channels at 44.1kHz _should_ be feasible with
  RP2350's ADCs



