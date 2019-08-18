cmake_minimum_required(VERSION 3.13)

add_library(common "")

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/source/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/source/*.h")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")
target_sources(common PRIVATE ${sources} PUBLIC ${headers})

set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)
