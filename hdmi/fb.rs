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
  pub pixels: [[Color; 400]; 240]
}

impl FrameBuffer {
  pub fn rect(self: &mut FrameBuffer, x: usize, y: usize, w: usize, h: usize, color: Color) -> () {
    self.pixels[y][x] = color;
    self.pixels[y + h-1][x + w-1] = color;
  }
}
