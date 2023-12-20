#!/bin/bash

cd ..
rm -r tmp_build
mkdir tmp_build
cd tmp_build
cmake .. -G Ninja
ninja
cd ..
./tmp_build/src/01/CUDA_Training_01_exe

