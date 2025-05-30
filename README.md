# hdmi2350

<p style="color:#ff0">
<b>(Work still in progress)</b>
</p>

* HDMI output device and frame buffer
* SPI input (peripheral) interface for image data
* 864 x 486 @ 60p


# HDMI Generator

Until this thing is able to change resolutions on the fly,
we have this one fixed resolution of `864 x 486 @ 60p`.

This is the best resolution I could find that fit some constraints:
* Perfectly 16:9 aspect ratio
* Framebuffer of ~ 420kB fits in RP2350 main SRAM (512kB),
  using RGB332
* It's near the "target" line count of 480
  (supporting common modes like 640x480 and wider)
* Essentially the maximum amount of detail possibly in
  NTSC (standard-def) video

Further, this mode's pixel clock is near 30 MHz.
If we tweak the HBlank period to lengthen the time of each line,
we can then achieve a *range* of frames per second,
around ±1% (59 to 61 fps).

I queried
[video_timings_calculator](https://tomverbeure.github.io/video_timings_calculator)
with the parameters for `864 x 486 @ 60p`.

I found these custom HBlank values are able to achieve
varying frame rates.  Note that this calculator adjusts
the pixel clock when you change HBlank, because it keeps
the VFreq at what you requested (at 60).
I'm assuming that if we hold the clock fixed at 30 MHz,
then we can tweak the frame rate instead.

| HBlank period (pixel-times) | PixelClock MHz (@ fixed 60Hz frame rate) | VFreq (@ fixed 30.0MHz px clock) |
|--|--|--|
| 126 | 29.7 | 59.4 |
| 136 | 30.0 | 60.0 |
| 146 | 30.3 | 60.6 |

This allows us to tweak our timing very slightly
(less than 1% in practice) in case we need to match
another video source with a clock we can't control.
For example this allows for an NTSC-to-HDMI converter
generating frames at a rate almost perfectly matching the source
(say, a composite video signal from an NES).

I'm *expecting* that such fudging of line timings
won't cause instability or incompatibility with the receiver,
since this modifies HBlank (porches and HSync) only a little,
while preserving vsync and vblank line counts.

(Eventually this should allow for self-governing or PID-controlling
the output line or frame rate.  But right now we're only targeting 60.0 fps.)

## HDMI Timing

The following is the result of our calculations,
with thanks again going to `@tomverbeure` for the [video_timings_calculator](https://tomverbeure.github.io/video_timings_calculator).

|                    | fixed   | variable          | |
|------------------- | -------------|-----------|---|
| AspectRatio       | 16:9 |
| PixelClock | 30  | | MHz |
| VActive | 486 | | Lines |
| VBlank | 14 | | Lines |
| VFront | 3 | | Lines |
| VSync | 5 | | Lines |
| VBack | 6 | | Lines |
| VTotal | 500 | | Lines |
| VFreq | 60 | *59.4 .. 60.6* | Hz |
| VPeriod | 16.667 | | ms |
| HActive | 864 || Pixels |
| HBlank | 136 Pixels | *126 .. 146* | Pixels |
| HFront | 40 |  | Pixels |
| HSync | 32 |   | Pixels |
| HBack | 64 | $`HBlank-HFront-HSync`$ | Pixels |
| HTotal | 1000 | *990 .. 1010* | Pixels |
| HFreq | 30 | *30.3 .. 29.7* | KHz |
| HPeriod | 33.333 | *33.0 .. 33.666* | µs |

Notes:
* Assumes fixed `HBlank` to get the 60Hz frame rate at 30MHz pixel clock.
* HFront/Back/Sync ratios taken from CVT-RB column.
* Effective HBlank (sync and porches) are within the
  CVT-RB and CVT-RBv2 values, which vary a lot, so I
  assume these have decent compatibility with receivers.


# HDMI Pixel Map

The resulting `500,000` pixels (1000 wide * 500 high),
consisting of control signals, data signals, and actual video pixels,
are arranged like so.  They're transmitted top-down and left-right.

![](docs/pixeltimings.svg)

Notes:
* As mentioned above, different HBackPorch periods can be
  used to adjust actual frame rate, and this image only reflects
  the target of 60.0 Hz.


# Links

https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf

## ARM
https://developer.arm.com/documentation/100235/0100/The-Cortex-M33-Processor/Exception-model/Vector-table

## Rust, including on ARMs or embedded
https://thejpster.org.uk/blog/blog-2024-08-08/
https://os.phil-opp.com/minimal-rust-kernel/
https://doc.rust-lang.org/nomicon/vec/vec-layout.html

## HDMI video mode
https://www.reddit.com/r/raspberrypipico/comments/1fj0vxg/can_you_get_169_video_with_hstx/
https://tomverbeure.github.io/video_timings_calculator

## Specs for DVI and HDMI, which somehow are publicly available
* DVI 1.0: https://www.cs.unc.edu/Research/stc/FAQs/Video/dvi_spec-V1_0.pdf
* HDMI 1.4: https://forums.parallax.com/discussion/download/128730/Hdmi-1.4-1000008562-6364143185282736974850538.pdf


# Misc

This project's documentation and code mention the term `hdmi`
a lot, but this is not guaranteed to be strictly HDMI compliant,
and is of course not licensed by "HDMI Licensing, LLC".
No warranties, no refunds, not for use in life-saving devices.



# License

Open source and made available under the MIT License.
