#ifndef SANE_SANEPP_RUSTY_DB_H
#define SANE_SANEPP_RUSTY_DB_H

/**
 * Because C++ has no stable ABI for the Rust compiler to target, we use C for any interoperability
 * between different languages. This is no exception when using Rust inside of C and C++ code.
 */

void rust_function();

#endif //SANE_SANEPP_RUSTY_DB_H
