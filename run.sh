#!/bin/bash

make -j$( nproc ) release && dslink test/libdsi_test.nds
