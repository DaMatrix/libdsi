set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DARM7")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

add_executable(arm7 "")
set_target_properties(arm7 PROPERTIES LINKER_LANGUAGE CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/source/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/source/*.c" "${CMAKE_CURRENT_LIST_DIR}/source/*.h" "${CMAKE_CURRENT_LIST_DIR}/source/*.S")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")
target_sources(arm7 PRIVATE ${sources} PUBLIC ${headers})

target_link_libraries(arm7 common)
