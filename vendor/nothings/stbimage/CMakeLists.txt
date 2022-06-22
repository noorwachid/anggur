cmake_minimum_required(VERSION 3.3)

project(Nothings::StbImage)

add_library(StbImage STATIC
        src/stb_image.c
        src/stb_image_write.c
)

target_include_directories(StbImage PUBLIC src)
