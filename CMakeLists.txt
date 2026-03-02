# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.25)

# Specify the project name
project(CSV_PrettyPrinter C)

# Set the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Specify the source files with the new path
set(SOURCES
    src/main.c
    src/file.c
    src/column.c
    src/row.c
)

# Add an executable
add_executable(CSV_PrettyPrinter ${SOURCES})