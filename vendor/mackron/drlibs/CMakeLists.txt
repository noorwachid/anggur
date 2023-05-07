cmake_minimum_required(VERSION 3.3)

project(drlibs)

add_library(drlibs STATIC
	"src/dr_wav.h"
	"src/dr_wav.c"
)

target_include_directories(drlibs PUBLIC "src")
 
