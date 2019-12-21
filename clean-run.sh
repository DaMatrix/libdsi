#!/bin/bash

make clean && make -j$( nproc ) && desmume test/test.nds
