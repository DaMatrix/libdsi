cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/PorkLib_cpp)

project(libdsi-root)

add_subdirectory(dsi)
add_subdirectory(link)
add_subdirectory(tool)
