cmake_minimum_required(VERSION 3.3)

project(STB::TrueType)

add_library(stb_truetype STATIC
        src/stb_truetype.c
)

target_include_directories(stb_truetype PUBLIC src)
