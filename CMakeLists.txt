cmake_minimum_required(VERSION 3.16)

project(sistema_de_gerenciamento VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(sistema-gerenciamento
    src/main.cpp
    src/atriz_repository.cpp
)

target_include_directories(sistema-gerenciamento PRIVATE include)

if(MSVC)
    target_compile_options(sistema-gerenciamento PRIVATE /W4)
else()
    target_compile_options(sistema-gerenciamento PRIVATE -Wall -Wextra -pedantic)
endif()
