#This project is configured with CMake, but don't expect CMake builds to actually work! It's only for integration with CLion, use the
#makefile in the project root to build.
cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

project(libdsi)

# Set up devkitPro variable stuff up
set(DEVKITPRO $ENV{DEVKITPRO})
set(LIBNDS ${DEVKITPRO}/libnds)
set(LIBNDS_INCLUDE ${LIBNDS}/include)
set(LIBNDS_LINK ${LIBNDS}/lib)

# Set C/C++ compiler flags
set(CMAKE_C_FLAGS "-DDSI")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

# Include libnds
include_directories(${LIBNDS_INCLUDE})
link_directories(${LIBNDS_LINK})

add_subdirectory(common)
add_subdirectory(arm7)
add_subdirectory(arm9)

add_subdirectory(test)
