#![no_std]
#![crate_type="lib"]

#[path = "../runtime/cxx.rs"]
pub mod cxx;

mod fb;

#[unsafe(link_section = ".framebuf")]
static mut FB: fb::FrameBuffer =
  fb::FrameBuffer {pixels: [[fb::Color::new(0, 0, 0); 400]; 240]};

#[no_mangle]
pub fn colorbars() -> () {
  unsafe {
    #[allow(static_mut_refs)]
    FB.rect(0, 0, 400, 300, fb::Color::new(1, 1, 1))
  }
}
