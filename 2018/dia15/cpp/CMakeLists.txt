cmake_minimum_required(VERSION 4.0.0)

project(
    advent_of_code
    DESCRIPTION "This is a project description."
    LANGUAGES CXX
)

add_executable(
    main
)

target_compile_features(
    main
    PRIVATE
    cxx_std_23
)

target_sources(
    main
    PRIVATE
    ./main.cpp
)

target_include_directories(
    main
    PRIVATE
    ~/.local/include
)

target_compile_options(
    main
    PRIVATE
    -fdiagnostics-color=always
    -Wall
    -Wextra
    # -Og
    -O3
    -march=native
)

target_link_directories(
    main
    PRIVATE
    ~/.local/lib
)

target_link_libraries(
    main
    PRIVATE
    -lcrypto
)

target_link_options(
    main
    PRIVATE
)
