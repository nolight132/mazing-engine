#! /bin/bash
ninja clean-all
mkdir build
cd build
cmake -G Ninja ..
ninja mazing-engine-debug
bin/mazing-engine-debug $*
cd ../
