set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DARM9")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

add_executable(arm9 "")
set_target_properties(arm9 PROPERTIES LINKER_LANGUAGE CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/source/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/source/*.h")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")
target_sources(arm9 PRIVATE ${sources} PUBLIC ${headers})

target_link_libraries(arm9 common)
