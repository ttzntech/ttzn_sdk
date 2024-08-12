cmake_minimum_required(VERSION 3.5)
project(demo)

# compile options
add_compile_options(-std=c++11)

find_package(ttzn_sdk REQUIREED)

######## CODY DEMO #########
add_executable(cody_demo cody_demo.cpp)
target_link_libraries(cody_demo ttzn_sdk)