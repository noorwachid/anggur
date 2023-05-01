cmake_minimum_required(VERSION 3.3)

project(stb::truetype)

add_library(stbtruetype STATIC
	"src/stb_truetype.c"
)

target_include_directories(stbtruetype PUBLIC "src")
