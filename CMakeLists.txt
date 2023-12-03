cmake_minimum_required(VERSION 3.21)

project(Selene)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "-std=c++20")
else()
    set(CXX_STANDARD 20)
    set(CXX_STANDARD_REQUIRED ON)
endif()

file(GLOB_RECURSE SRC src/*.cpp include/*.hpp)

add_library(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PUBLIC include/)

add_subdirectory(Lua)
target_link_libraries(${PROJECT_NAME} PRIVATE Lua)