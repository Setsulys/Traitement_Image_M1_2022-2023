cmake_minimum_required(VERSION 3.13)

# give a name to the project
project(fourier)

# find opencv
find_package(OpenCV REQUIRED)

# compilation flags
set(CMAKE_CXX_FLAGS "-Wall -g -O2 -std=c++14")

# put the binary files in this directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# create a directory called 'output' in the project binary directory
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)

# the files to compile
add_executable(fourier src/fourierTransform.cpp src/fourierTransform.hpp src/tp05_ex01.cpp)
target_link_libraries(fourier ${OpenCV_LIBS})
