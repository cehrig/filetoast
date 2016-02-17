#!/bin/sh
autoreconf --force --install -I config -I m4
mkdir build && cd build
../configure
make
make install
