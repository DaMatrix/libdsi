set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DARM7")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

add_executable(test_arm7 "")
set_target_properties(test_arm7 PROPERTIES LINKER_LANGUAGE CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/source/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/source/*.h")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")
target_sources(test_arm7 PRIVATE ${sources} PUBLIC ${headers})

target_link_libraries(test_arm7 test_common)
