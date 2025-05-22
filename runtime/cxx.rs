// Shims to join Rust code to C++ code.

//
// Strings
//

#[repr(C)]
pub struct String {
  size: usize,
  data: *const u8,
}

impl String {
  pub fn new(src: &str) -> Self {
    String {size: src.len(), data: src.as_ptr()}
  }
}

//
// Panic handler
//

extern "C" {
// Matches the `__panic` in `runtime/Panic.h`
pub fn __panic(r#type: String,
               msg: String,
               loc_str: String,
               arg1: u32, arg2: u32, arg3: u32);
}

use core::panic::PanicInfo;
#[panic_handler]
#[inline(never)]
pub fn panic(info: &PanicInfo) -> ! {
  let mut loc_file = "";
  let mut loc_line = 0;
  if let Some(loc) = info.location() {
    loc_file = loc.file();
    loc_line = loc.line();
  }

  unsafe {
    __panic(
      String::new("rust_panic"),
      String::new("rust_panic"),  // XXX payload or msg or something, from PanicInfo
      String::new(loc_file),
      loc_line, 0, 0);

    loop {}  // __panic should not have returned; if it did, just halt
  }
}

