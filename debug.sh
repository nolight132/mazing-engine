#! /bin/bash
ninja clean-all
mkdir build
cd build
cmake -G Ninja ..
ninja
bin/mazing-engine
cd ../