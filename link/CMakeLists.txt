project(dsilink)

# Set C/C++ compiler flags
set(CMAKE_C_FLAGS "-O3")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

add_executable(dsilink "")
set_target_properties(dsilink PROPERTIES LINKER_LANGUAGE CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/source/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/source/*.h" "${CMAKE_CURRENT_LIST_DIR}/source/*.S")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")
target_sources(dsilink PRIVATE ${sources} PUBLIC ${headers})

target_link_libraries(dsilink porklib_cpp)
