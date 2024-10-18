cmake_minimum_required(VERSION 3.8)

project(demo LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cu"
)

add_executable(demo ${SRC})

target_include_directories(demo PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(demo PRIVATE Physics cudart cublas rt pthread dl)

set_property(TARGET Physics PROPERTY CUDA_SEPARABLE_COMPILATION ON)  # Enable separable compilation for Physics if needed
