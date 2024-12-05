cmake_minimum_required(VERSION 3.1)
project(HERO 
LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(reorder src/reorder.cpp src/graph.cpp src/util.cpp)
add_executable(tc src/tc.cpp src/graph.cpp src/SIBTree.cpp)
add_executable(mc src/mc.cpp src/graph.cpp src/SIBTree.cpp)
add_executable(sl src/sl.cpp src/graph.cpp src/SIBTree.cpp)
add_executable(pt src/pt.cpp src/graph.cpp src/SIBTree.cpp)
