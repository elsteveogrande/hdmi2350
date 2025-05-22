// We're going bare-metal so we want `core` only
#![no_std]                    

// Produce an `.a` archive, containing runtime code as well as `core`
#![crate_type="staticlib"]    

pub mod cxx;
