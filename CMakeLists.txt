cmake_minimum_required(VERSION 3.24)
project(CUDA_Training)

##################################################
##### Envirionment Variables #####################
##################################################
##### Set environment variables in project
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external)

message("PROJECT_INCLUDE_DIR=" ${PROJECT_INCLUDE_DIR})
message("PROJECT_EXTERNAL_DIR=" ${PROJECT_EXTERNAL_DIR})

##################################################
##### External Libraries #########################
##################################################
##### CUDA Toolkit
find_package(CUDAToolkit REQUIRED)

##### OpenMP
find_package(OpenMP)

##### OpenGL
find_package(OpenGL REQUIRED)

##### GLFW
set(GLFW_INCLUDE_DIR ${PROJECT_EXTERNAL_DIR}/glfw-3.3.9/include)
set(GLFW_LIB_DIR ${PROJECT_EXTERNAL_DIR}/glfw-3.3.9/lib)

##### GLAD
set(GLAD_INCLUDE_DIR ${PROJECT_EXTERNAL_DIR}/glad/include)

set(LIB_GLAD glad)
add_library(
  ${LIB_GLAD}
  STATIC
  ${PROJECT_EXTERNAL_DIR}/glad/src/glad.c
)
target_include_directories(
  ${LIB_GLAD}
  PUBLIC
  ${GLAD_INCLUDE_DIR}
)

##### Dear Imgui
set(IMGUI_INCLUDE_DIR ${PROJECT_EXTERNAL_DIR}/imgui)
set(IMGUI_SOURCE_DIR ${PROJECT_EXTERNAL_DIR}/imgui)
set(IMGUI_BACKENDS_DIR ${IMGUI_SOURCE_DIR}/backends)

##### X11
find_package(X11 REQUIRED)

set(
  IMGUI_SOURCE_FILES
  ${IMGUI_SOURCE_DIR}/imgui.cpp
  ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
  ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
  ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
  ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
  ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
  ${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp
  ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
)

##### stb
set(STB_INCLUDE_DIR ${PROJECT_EXTERNAL_DIR}/stb)

##### Add subdirectory
add_subdirectory("src")

##################################################
##### Message ####################################
##################################################
message("# ======================================================================================================")
message("#  ${PROJECT_NAME} build configuration")
message("# ======================================================================================================")
message("#  CMake version                    : ${CMAKE_VERSION}")
message("#  CMake build type                 : ${CMAKE_BUILD_TYPE}")
message("# ")
message("#  C++ Compiler                     : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} ${CMAKE_CXX_COMPILER_WRAPPER} ${CMAKE_CXX_COMPILER}")
message("#  CXX      STANDARD                : ${CMAKE_CXX_STANDARD}")
message("# ")
message("#  CUDA compiler ID                 : ${CMAKE_CUDA_COMPILER_ID}")
message("#  CUDA compiler Version            : ${CMAKE_CUDA_COMPILER_VERSION}")
message("#  CUDA CXX STANDARD                : ${CMAKE_CUDA_STANDARD}")
message("#  CUDA Compiler                    : ${CMAKE_CUDA_COMPILER}")
message("#  CUDA Compiler exec               : ${CUDA_NVCC_EXECUTABLE}")
message("#  CUDA Compile flags               : ${CMAKE_CUDA_FLAGS}")
message("# ")
message("#  OpenMP compile flags             : ${OpenMP_CXX_FLAGS}")
message("# ")
message("#  PROJECT_INCLUDE_DIR              : ${PROJECT_INCLUDE_DIR}")
message("#  PROJECT_EXTERNAL_DIR             : ${PROJECT_EXTERNAL_DIR}")
message("# ")
message("#  GLFW_INCLUDE_DIR                 : ${GLFW_INCLUDE_DIR}")
message("#  GLFW_LIB_DIR                     : ${GLFW_LIB_DIR}")
message("#  GLAD_INCLUDE_DIR                 : ${GLAD_INCLUDE_DIR}")
message("#  IMGUI_INCLUDE_DIR                : ${IMGUI_INCLUDE_DIR}")
message("#  IMGUI_SOURCE_DIR                 : ${IMGUI_SOURCE_DIR}")
message("#  IMGUI_BACKENDS_DIR               : ${IMGUI_BACKENDS_DIR}")
message("#  STB_INCLUDE_DIR                  : ${STB_INCLUDE_DIR}")
message("# ======================================================================================================")
