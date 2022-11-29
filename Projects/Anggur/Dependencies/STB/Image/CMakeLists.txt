cmake_minimum_required(VERSION 3.3)

project(STB::Image)

add_library(STB_Image STATIC
        src/stb_image.c
        src/stb_image_write.c
)

target_include_directories(STB_Image PUBLIC src)
