cmake_minimum_required(VERSION 3.3)

project(nothings::stbimage)

add_library(stbimage STATIC
        src/stb_image.c
        src/stb_image_write.c
)

target_include_directories(stbimage PUBLIC src)
