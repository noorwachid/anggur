cmake_minimum_required(VERSION 3.3)

project(STB::Image)

add_library(stb_image STATIC
        src/stb_image.c
        src/stb_image_write.c
)

target_include_directories(stb_image PUBLIC src)
