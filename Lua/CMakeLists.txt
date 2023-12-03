cmake_minimum_required(VERSION 3.21)

project(Lua)

set(LUA_VERSION "5.4.6")
set(SRC_DIR "lua-${LUA_VERSION}")

file(GLOB_RECURSE SRC ${SRC_DIR}/src/*.c ${SRC_DIR}/src/*.h)
add_library(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/src/)