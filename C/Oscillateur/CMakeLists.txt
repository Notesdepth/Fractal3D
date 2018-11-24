cmake_minimum_required(VERSION 3.12.2)
project(Oscillateur)

set(CMAKE_CXX_STANDARD 14)

set(EXEC Oscillateur)

set(HEADER
        include/oscillateur.h)

set(SOURCES
        src/main.c
        src/init.c)

add_executable(${EXEC} ${SOURCES} ${HEADER})

target_include_directories(${EXEC} PRIVATE include)