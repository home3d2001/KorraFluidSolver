#!/bin/sh
# clone submodules
git submodule update --init --recursive

# compile nanogui
cd src/thirdparty/nanogui
cmake -G "Unix Makefiles" .
make

# compile Korra
cd ../../../
mkdir build
cd build
cmake -G "Unix Makefiles" ../
make

#launch
./Korra
