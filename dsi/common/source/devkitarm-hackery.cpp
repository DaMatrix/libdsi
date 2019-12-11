#include <dsi.h>


bool __dsimode; //never actually accessed by user, should never be false

extern "C" const int __secure_area__ = 0;

extern "C" void __libnds_exit(int code) {
    //TODO: this should do something...
    while (true);
}
