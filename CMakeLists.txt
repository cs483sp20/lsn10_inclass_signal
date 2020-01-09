cmake_minimum_required(VERSION 3.13)
project(signalExample C)

set(CMAKE_C_STANDARD 11)

add_executable(signalExample
        tester.c)
target_link_libraries(signalExample pthread)