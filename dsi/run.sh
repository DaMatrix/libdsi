#!/bin/bash

#make -j$( nproc ) release && dslink test/libdsi_test.nds
make release && dslink test/libdsi_test.nds
