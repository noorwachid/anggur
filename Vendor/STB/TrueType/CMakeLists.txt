cmake_minimum_required(VERSION 3.3)

project(STB::TrueType)

add_library(STBTrueType STATIC
	Source/stb_truetype.c
)

target_include_directories(STBTrueType PUBLIC Source)
