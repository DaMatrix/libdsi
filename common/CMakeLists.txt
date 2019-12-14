add_library(common "")
set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/source/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/source/*.h" "${CMAKE_CURRENT_LIST_DIR}/source/*.S")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/../include/*.h")
target_sources(common PRIVATE ${sources} PUBLIC ${headers})

target_include_directories(common PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include")
