project(demo)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(demo ${SRC})

# Include directories
target_include_directories(demo PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(demo PRIVATE Physics)