cmake_minimum_required(VERSION 3.10)
project(image_toolkit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(image-toolkit main.cpp image.cpp)
target_include_directories(image-toolkit PRIVATE ${CMAKE_SOURCE_DIR}/src)
