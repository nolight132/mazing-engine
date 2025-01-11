#! /bin/bash
ninja clean-all
mkdir build
cd build
cmake -G Ninja ..
ninja mazing-engine-release
if [ $? -eq 0 ]; then
    bin/mazing-engine-release $*
fi
cd ../
