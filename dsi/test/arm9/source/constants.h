#ifndef TEMPLATE_CONSTANTS_H
#define TEMPLATE_CONSTANTS_H

#include <test_common.h>

#include <nds.h>
#include <cstdarg>
#include <cstdio>
#include "console.h"

const unsigned short SERVER_PORT = 8236;

//some math functions
int min(int a, int b);
int max(int a, int b);
int clamp(int a, int min, int max);

const char* fmt(const char* format, ...);

#endif //TEMPLATE_CONSTANTS_H
