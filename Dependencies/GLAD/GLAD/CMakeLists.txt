cmake_minimum_required(VERSION 3.3)

project(GLAD)

add_library(GLAD STATIC
	include/glad/glad.h
	src/glad.c
)

target_include_directories(GLAD PUBLIC include)
