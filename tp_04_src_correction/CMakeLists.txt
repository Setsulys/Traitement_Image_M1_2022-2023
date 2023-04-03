cmake_minimum_required(VERSION 3.15)

# give a name to the project
project(tp04)

# find opencv
find_package(OpenCV REQUIRED)


# or the automatic version
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# for each sample file, make an exe
foreach(SRC_FILE ${SRC_FILES})
    get_filename_component(FILE ${SRC_FILE} NAME_WE)
    add_executable(${FILE} ${SRC_FILE})
    target_link_libraries(${FILE} ${OpenCV_LIBS})

    # compilation options
    target_compile_features(${FILE} PRIVATE cxx_std_17) # use at least c++ 17
    if (MSVC)
        target_compile_options(${FILE} PRIVATE /W3)
    else()
        target_compile_options(${FILE} PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors)
    endif()
endforeach()


