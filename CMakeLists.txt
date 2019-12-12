project(genetic_algorithm)
cmake_minimum_required(VERSION 2.8.3)
add_compile_options(-std=c++11)

include_directories(.)

add_library(${PROJECT_NAME} chromosome.h ga_string.h genetic_algorithm.h)
target_link_libraries(${PROJECT_NAME} armadillo)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_executable(test main.cpp)

target_link_libraries(test genetic_algorithm)
