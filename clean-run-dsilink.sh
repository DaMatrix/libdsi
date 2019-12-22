#!/bin/bash

make clean && make -j$( nproc ) && dsilink test/test.nds
