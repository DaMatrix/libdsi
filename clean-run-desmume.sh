#!/bin/bash

make clean && make -j$( nproc ) && desmume test/test.nds
#make clean && make && desmume test/test.nds
