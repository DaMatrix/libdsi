# dsilink

A re-implementation of [dslink](https://davejmurphy.com/dslink/) intended to work on machines with multiple network interfaces (for example, you're running a WiFi hotspot on your development machine for your DS to connect to).

## Build instructions

`sudo apt install git cmake build-essential`

`git clone --recursive https://github.com/DaMatrix/libdsi.git`

`mkdir libdsi/build && cd libdsi/build`

`cmake ..`

`make -j$( nproc ) dsilink`

The final executable will be placed in `./dsilink/dsilink`.
