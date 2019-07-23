#ifndef SANE_SANEPP_RUSTY_DB_H
#define SANE_SANEPP_RUSTY_DB_H

/**
 * Because C++ has no stable ABI for the Rust compiler to target, we use C for any interoperability
 * between different languages. This is no exception when using Rust inside of C and C++ code.
 *
 * https://rust-embedded.github.io/book/interoperability/rust-with-c.html
 *
 * Additionally, name mangling in C++ is not standardized, therefore void <funcname>()
 * might have a different linkage compared to C. You can force the same C linkage in both
 * languages by using extern "C" as part of the functions signature/prototype
 *
 * https://stackoverflow.com/a/50234125
 */

extern "C" void rust_function();

#endif //SANE_SANEPP_RUSTY_DB_H