cmake_minimum_required(VERSION 3.3)

project(stb)

add_library(stb STATIC
        stb_image.c
        stb_image_write.c
        stb_truetype.c
        stb_rect_pack.c
)

target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
