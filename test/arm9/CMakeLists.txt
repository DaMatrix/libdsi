set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DARM9")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

add_executable(test_arm9 "")
set_target_properties(test_arm9 PROPERTIES LINKER_LANGUAGE CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/source/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/source/*.c" "${CMAKE_CURRENT_LIST_DIR}/source/*.h" "${CMAKE_CURRENT_LIST_DIR}../test_common/source/*.h" "${CMAKE_CURRENT_LIST_DIR}/source/*.S")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")
target_sources(test_arm9 PRIVATE ${sources} PUBLIC ${headers})

target_link_libraries(test_arm9 test_common)
