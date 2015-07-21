cmake_minimum_required(VERSION 2.8.12.2)
project(pong)

# use modified FindSDL2* modules
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${pong_SOURCE_DIR}/cmake")
# set an output directory for binaries
#set(BIN_DIR ${pong_SOURCE_DIR}/bin)
# 
# set(SRC_DIR ${pong_SOURCE_DIR}/src)

# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused -pedantic -std=c++11 -m32")
    set(CMAKE_CXX_FLAGS_DEBUG "$(CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "$(CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

# look up SDL2 and add the include directory to our include PATHfind_package(SDL2 REQUIRED)
find_package(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIR})
include_directories(include)
link_directories(${SDL_LIBRARY})

set(SDL_LIBRARIES "SDL2")

if(${WIN32})
    set(SDL_LIBRARIES "mingw32;SDL2main;${SDL_LIBRARIES};SDL2_image;SDL2_ttf")
endif()

add_executable(pong
    src/main.cpp
    src/display.cpp
    src/player.cpp
    src/ball.cpp
    src/texture.cpp
)
target_link_libraries(pong ${SDL_LIBRARIES})
