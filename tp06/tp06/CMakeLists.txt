cmake_minimum_required(VERSION 3.15)

# give a name to the project
project(morpho)

# find opencv
find_package(OpenCV REQUIRED)

# the files to compile
add_executable(morpho src/morpho.cpp)
target_link_libraries(morpho ${OpenCV_LIBS})


# compilation options
target_compile_features(morpho PRIVATE cxx_std_17) # use at least c++ 17
if (MSVC)
    target_compile_options(morpho PRIVATE /W3)
else()
    target_compile_options(morpho PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors)
endif()



