cmake_minimum_required(VERSION 3.30)
project(TO_PRINT)

set(CMAKE_CXX_STANDARD 20)

add_executable(TO_PRINT main.cpp
        SortedUniqueVectoredList.h
        SortedUniqueVectoredList.cpp)
