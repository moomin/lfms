#!/bin/sh

rm -fr ./build
mkdir build
cd build
cmake -G "Unix Makefiles" ../ && make
