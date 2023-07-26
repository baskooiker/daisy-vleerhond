cmake_minimum_required(VERSION 3.16.3)

project(vleerhond_test)

set(CMAKE_CXX_STANDARD 14)

add_library(vleerhond_lib SHARED 
    ../vleerhond_lib/core/time_struct.cpp 
    ../vleerhond_lib/core/time_struct.h
    ../vleerhond_lib/patterns/ab.cpp 
    ../vleerhond_lib/patterns/ab.h
    ../vleerhond_lib/patterns/cv_patterns.cpp 
    ../vleerhond_lib/patterns/cv_patterns.h
    ../vleerhond_lib/utils/rand.cpp
    ../vleerhond_lib/utils/rand.h
    ../vleerhond_lib/utils/utils.cpp
    ../vleerhond_lib/utils/utils.h
)

include_directories(../vleerhond_lib)

add_executable(test_cv_patterns test_cv_patterns.cpp)
target_link_libraries(test_cv_patterns vleerhond_lib)
