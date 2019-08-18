#!/bin/bash

make -j$( nproc ) release && dslink test/dsiwifi_test.nds
