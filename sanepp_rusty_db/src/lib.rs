mod db;

// The Rust compiler mangles symbol names differently than native code linkers expect.
// As such, any function that Rust exports to be used outside of Rust needs to be told
// not to be mangled by the compiler.
#[no_mangle]

// By default, any function you write in Rust will use the Rust ABI (which is also not stabilized).
// Instead, when building outwards facing FFI APIs we need to tell the compiler to use
// the system ABI.
pub extern "C" fn test_rust_linkage<'a>() -> &'a str {
    return "Tested rust linkage";
}

pub extern "C" fn run_example() {
    db::example()
}