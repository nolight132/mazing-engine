#! /bin/bash
ninja clean-all
mkdir build
cd build
cmake -G Ninja ..
ninja mazing-engine-release
bin/mazing-engine-release
cd ../
