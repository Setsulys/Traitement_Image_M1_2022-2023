cmake_minimum_required(VERSION 2.8)

# give a name to the project
project(fourier)

# find opencv
find_package(OpenCV REQUIRED)

# check opencv version
if(${OpenCV_VERSION} VERSION_LESS 2.0.0)
message(FATAL_ERROR “OpenCV version is not compatible : ${OpenCV_VERSION}”)
endif()

# compilation flags
set(CMAKE_CXX_FLAGS "-Wall -g -O2 -std=c++11")

# put the binary files in this directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# create a directory called 'output' in the project binary directory
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)

# the files to compile
add_executable(fourier src/fourierTransform.cpp src/fourierTransform.hpp src/fourier.cpp)
target_link_libraries(fourier ${OpenCV_LIBS})

add_executable(convolutions src/fourierTransform.cpp src/fourierTransform.hpp src/convolutions.cpp)
target_link_libraries(convolutions ${OpenCV_LIBS})
