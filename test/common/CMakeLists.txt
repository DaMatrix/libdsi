add_library(test_common "")
set_target_properties(test_common PROPERTIES LINKER_LANGUAGE CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/source/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/source/*.c" "${CMAKE_CURRENT_LIST_DIR}/source/*.h" "${CMAKE_CURRENT_LIST_DIR}/source/*.S")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")
target_sources(test_common PRIVATE ${sources} PUBLIC ${headers})

target_link_libraries(test_common common)

target_include_directories(test_common PUBLIC "${CMAKE_CURRENT_LIST_DIR}/source")
