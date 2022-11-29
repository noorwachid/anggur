cmake_minimum_required(VERSION 3.3)

project(STB::TrueType)

add_library(STB_TrueType STATIC
        src/stb_truetype.c
)

target_include_directories(STB_TrueType PUBLIC src)
