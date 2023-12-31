cmake_minimum_required(VERSION 3.24)
project(CUDA-CMake-Minimal-Template)

# #################################################
# #### Compile Options ############################
# #################################################
set(CMAKE_CXX_FLAGS "-std=c++17")
set(CMAKE_CUDA_FLAGS "-arch=sm_86")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall")
set(CMAKE_CUDA_FLAGS_DEBUG "-g -O0 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CUDA_FLAGS_RELEASE "-O3 -DNDEBUG")

# #################################################
# #### Envirionment Variables #####################
# #################################################
# #### Set environment variables in project
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external)

message("PROJECT_INCLUDE_DIR=" ${PROJECT_INCLUDE_DIR})
message("PROJECT_EXTERNAL_DIR=" ${PROJECT_EXTERNAL_DIR})

# #################################################
# #### External Libraries #########################
# #################################################
# #### CUDA Toolkit
find_package(CUDAToolkit REQUIRED)

# #### OpenMP
find_package(OpenMP)

# #### OpenGL
find_package(OpenGL REQUIRED)

# #### GLFW
set(GLFW_INCLUDE_DIR ${PROJECT_EXTERNAL_DIR}/glfw/include)

# #### GLAD
set(GLAD_INCLUDE_DIR ${PROJECT_EXTERNAL_DIR}/glad/include)

# #### GLM
set(GLM_INCLUDE_DIR ${PROJECT_EXTERNAL_DIR}/glm)

# #### Dear Imgui
set(IMGUI_INCLUDE_DIR ${PROJECT_EXTERNAL_DIR}/imgui)
set(IMGUI_SOURCE_DIR ${PROJECT_EXTERNAL_DIR}/imgui)
set(IMGUI_BACKENDS_DIR ${IMGUI_SOURCE_DIR}/backends)

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

# #### X11
if(UNIX)
  find_package(X11 REQUIRED)
endif(UNIX)

# #### stb
set(STB_INCLUDE_DIR ${PROJECT_EXTERNAL_DIR}/stb)

# #### Add subdirectory
add_subdirectory("src")
add_subdirectory("external")

# #################################################
# #### Message ####################################
# #################################################
message("# =======================================================================================================")
message("#  '${PROJECT_NAME}' build configuration ")
message("# =======================================================================================================")
message("#  [CMake]")
message("#    CMake version            : ${CMAKE_VERSION}")
message("#    CMake build type         : ${CMAKE_BUILD_TYPE}")
message("# ")
message("#  [C/C++]")
message("#    C++ Compiler             : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} ${CMAKE_CXX_COMPILER_WRAPPER} ${CMAKE_CXX_COMPILER}")
message("#    CXX STANDARD             : ${CMAKE_CXX_STANDARD}")
message("# ")
message("#  [CUDA]")
message("#    CUDA compiler ID         : ${CMAKE_CUDA_COMPILER_ID}")
message("#    CUDA compiler Version    : ${CMAKE_CUDA_COMPILER_VERSION}")
message("#    CUDA CXX STANDARD        : ${CMAKE_CUDA_STANDARD}")
message("#    CUDA Compiler            : ${CMAKE_CUDA_COMPILER}")
message("#    CUDA Compiler exec       : ${CUDA_NVCC_EXECUTABLE}")
message("#    CUDA Compile flags       : ${CMAKE_CUDA_FLAGS}")
message("# ")
message("#  [OpenMP]")
message("#    OpenMP compile flags     : ${OpenMP_CXX_FLAGS}")
message("# ")
message("#  [Environment Variables]")
message("#    PROJECT_INCLUDE_DIR      : ${PROJECT_INCLUDE_DIR}")
message("#    PROJECT_EXTERNAL_DIR     : ${PROJECT_EXTERNAL_DIR}")
message("# ")
message("#    GLFW_INCLUDE_DIR         : ${GLFW_INCLUDE_DIR}")
message("#    GLAD_INCLUDE_DIR         : ${GLAD_INCLUDE_DIR}")
message("#    GLM_INCLUDE_DIR          : ${GLM_INCLUDE_DIR}")
message("#    IMGUI_INCLUDE_DIR        : ${IMGUI_INCLUDE_DIR}")
message("#    IMGUI_SOURCE_DIR         : ${IMGUI_SOURCE_DIR}")
message("#    IMGUI_BACKENDS_DIR       : ${IMGUI_BACKENDS_DIR}")
message("#    STB_INCLUDE_DIR          : ${STB_INCLUDE_DIR}")
message("# =======================================================================================================")
