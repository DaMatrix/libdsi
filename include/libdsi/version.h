#ifndef LIBDSI_VERSION_H
#define LIBDSI_VERSION_H

namespace dsi {
    constexpr int VERSION_MAJOR    = 0;
    constexpr int VERSION_MINOR    = 0;
    constexpr int VERSION_REVISION = 0;
    constexpr int VERSION          = (VERSION_MAJOR * 1000 + VERSION_MINOR) * 1000 + VERSION_REVISION;

    constexpr const char* VERSION_STRING = "0.0.0-SNAPSHOT";
}

#endif // LIBDSI_VERSION_H
