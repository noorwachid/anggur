cmake_minimum_required(VERSION 3.3)

project(STB::Image)

add_library(STBImage STATIC
	Sources/stb_image.c
    Sources/stb_image_write.c
)

target_include_directories(STBImage PUBLIC Sources)
