cmake_minimum_required (VERSION 3.3)

project (Stb)

add_library (Stb STATIC
        stb_image.c
        stb_image_write.c
        stb_truetype.c
        stb_rect_pack.c
)
target_include_directories(Stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
