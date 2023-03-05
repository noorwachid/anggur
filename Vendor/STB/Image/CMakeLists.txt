cmake_minimum_required(VERSION 3.3)

project(STB::Image)

add_library(STBImage STATIC
	Source/stb_image.c
    Source/stb_image_write.c
)

target_include_directories(STBImage PUBLIC Source)
