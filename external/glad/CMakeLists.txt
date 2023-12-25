cmake_minimum_required(VERSION 3.24)

project(glad)

add_library(
  ${PROJECT_NAME}
  STATIC
  "src/gl.c"
)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  "include"
)
