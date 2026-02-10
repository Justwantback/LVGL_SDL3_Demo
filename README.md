# LVGLSDLDEMO

## Getting Started

### Ninja
https://github.com/ninja-build/ninja/releases


### Cmake
https://cmake.org/download/

reset:
cmake --fresh -S . -B build -G Ninja

clear:
cmake --build build --target clean

build:
cmake --build build
