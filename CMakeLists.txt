##################################################################################

cmake_minimum_required(VERSION 3.12)
project(StagDB_TestBed)

enable_language(C)
enable_language(CXX)

# ---[ C++ Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# FIXME set these flags later
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
include_directories(src)
include_directories(src/includes)

file(GLOB code_files
        "src/*.cpp"
        )

add_library(stagdb_lib ${code_files})
add_executable(stagdb_main src/main.cpp)

target_link_libraries(stagdb_main stagdb_lib)
set_target_properties(stagdb_main PROPERTIES ENABLE_EXPORTS on)

# ---[ CTest
include(CTest)
add_subdirectory(test)