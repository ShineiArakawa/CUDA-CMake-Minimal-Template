#!/bin/bash

cd ..
rm -r tmp_build
mkdir tmp_build
cd tmp_build
cmake .. -G Ninja
ninja
cd ..
./tmp_build/src/02/CUDA_Training_02_exe

