cmake_minimum_required(VERSION 3.9)
project(PythonModule)

set(CMAKE_CXX_STANDARD 11)

add_executable(PythonModule main.cpp IMUlib.h IMUlib.cpp MajvikFilter.cpp MajvikFilter.h Quaternion.cpp Quaternion.h PyIMU.cpp)