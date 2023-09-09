cmake_minimum_required(VERSION 3.3)

set(CMAKE_LANGUAGE 17)

project(stbvorbis)

add_library(stbvorbis STATIC
	"src/stb_vorbis.c"
)

target_include_directories(stbvorbis PUBLIC "src")
