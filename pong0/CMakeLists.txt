project(pong0)
add_executable(pong0 /src/main.cpp)
target_link_libraries(pong0 ${SDL2_LIBRARY})
install(TARGETS pong0 RUNTIME DESTINATION ${BIN_DIR})
