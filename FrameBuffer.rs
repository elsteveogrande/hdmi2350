#![no_main]
#![no_std]

// TODO: revisit panic handler

use core::panic::PanicInfo;
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
  loop {}
}

// use core::panic::PanicInfo;

// // The panic handler is in C++
// extern "C" {
// fn panic(r#type:u32, arg1:u32, arg2:u32, arg3:u32);
// }

// // Rust needs a panic handler; this one calls the "main" panic
// #[panic_handler]
// pub fn panic_rs(_info: &PanicInfo) -> ! {
//   unsafe { panic(0, 0, 0, 0); }
//   loop {}  // panic shouldn't return
// }

#[repr(C)]          // struct is "standard layout"
pub struct FB {
}

extern "C" {
#[unsafe(link_section = ".framebuf")]
pub static fb: FB;
}
