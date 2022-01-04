#!/bin/bash

./configure --enable-sim --with-expat --with-python --without-libunwind --with-tcl=/usr/lib/ --with-tk=/usr/lib/ --disable-gas --disable-binutils
make
make install 