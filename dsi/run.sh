#!/bin/bash

#make -j$( nproc ) && dsilink test/test.nds
make && dsilink test/test.nds
