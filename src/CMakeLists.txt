cmake_minimum_required(VERSION 3.8)

project(Physics LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)

file(GLOB_RECURSE SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cu"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cuh"
)

add_library(Physics ${SRC})

target_include_directories(Physics PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Util
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(Physics PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
)

# Link any required CUDA libraries (if needed)
# target_link_libraries(Physics PRIVATE cublas cudart rt pthread dl)
