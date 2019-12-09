#!/bin/bash

# libdsi install script

# constants
XPACK_VERSION="9.2.1-1.1"
XPACK_URL="https://github.com/xpack-dev-tools/arm-none-eabi-gcc-xpack/releases/download/v$XPACK_VERSION/xpack-arm-none-eabi-gcc-$XPACK_VERSION-linux-x64.tar.gz"

echo "libdsi will be installed to $( readlink -f . )"
read -p "Are you sure you want to continue? (y/n) " -n 1 -r && echo
if [[ ! $REPLY =~ ^[yY]$ ]]; then echo "Abort."; exit 1; fi

# clean up any temp directories
if [[ -d temp/ ]]; then echo "Old temp directory found, removing..."; rm -rf temp/; fi; mkdir temp/

if [[ ! -d gcc/ ]]; then
    echo "Downloading xPack GCC v$XPACK_VERSION"
    curl -o - -L "$XPACK_URL" | tar zxf - -C temp/
    mv "temp/xpack-arm-none-eabi-gcc-$XPACK_VERSION/" "gcc/"
    echo "GCC installed!"
fi

#TODO: rework this whole thing, i don't yet know how i'll deal with setting up the actual SDK in the future
# (this works for now, since it's just me using it for the test project)
