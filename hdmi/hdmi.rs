#![no_std]
#![crate_type="lib"]

#[path = "../runtime/cxx.rs"]
pub mod cxx;

#[repr(C)]
#[repr(packed(1))]
#[derive(Copy)]
#[derive(Clone)]
pub struct Color {
  bits: u8
}

impl Color {
  pub const fn new(r: u8, g: u8, b:u8) -> Self {
    Color {bits: (r << 5) | (g << 2) | b}
  }
}

#[repr(C)]
pub struct FrameBuffer {
  pub pixels: [[Color; 864]; 486],
}

impl FrameBuffer {
  pub fn rect(self: &mut FrameBuffer, x: usize, y: usize, w: usize, h: usize, color: Color) -> () {
    self.pixels[y][x] = color;
    self.pixels[y + h-1][x + w-1] = color;
  }

  pub fn colorbars(self: &mut FrameBuffer) -> () {
    let mut x: usize = 0;
    self.rect(x, 0, 108, 486, Color::new(2, 2, 1)); x += 108;
    self.rect(x, 0, 108, 486, Color::new(7, 7, 3)); x += 108;
    self.rect(x, 0, 108, 486, Color::new(7, 7, 0)); x += 108;
    self.rect(x, 0, 108, 486, Color::new(0, 7, 3)); x += 108;
    self.rect(x, 0, 108, 486, Color::new(0, 7, 0)); x += 108;
    self.rect(x, 0, 108, 486, Color::new(7, 0, 3)); x += 108;
    self.rect(x, 0, 108, 486, Color::new(7, 0, 0)); x += 108;
    self.rect(x, 0, 108, 486, Color::new(0, 0, 3));
  }
}

#[unsafe(link_section = ".framebuf")]
static mut FB: FrameBuffer =
  FrameBuffer {pixels: [[Color::new(0, 0, 0); 864]; 486]};

#[no_mangle]
pub fn colorbars() -> () {
  unsafe {
    #[allow(static_mut_refs)]
    FB.colorbars();
  }
}
