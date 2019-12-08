#!/bin/bash

#make -j$( nproc ) release && dsilink test/libdsi_test.nds
make release && dsilink test/libdsi_test.nds
