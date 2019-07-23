#!/bin/bash

# Database rust lib
root_dir="$(pwd)"
#set -e
cd sanepp_rusty_db/
# Export the cargo target dir variable (*only* inside subshell).
(export CARGO_TARGET_DIR="${root_dir}/lib/sanepp_rusty_db"; cargo build --lib)
cd ..