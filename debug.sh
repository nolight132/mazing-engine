#! /bin/bash
ninja clean-all
mkdir build
cd build
cmake -G Ninja ..
ninja mazing-engine-debug
if [ $? -eq 0 ]; then
    bin/mazing-engine-debug $*
fi
cd ../
