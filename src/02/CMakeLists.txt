project(CUDA_Training_02 LANGUAGES CUDA CXX)

set(LIB_PROJECT_NAME CUDA_Training_02_kernel)
set(EXE_PROJECT_NAME CUDA_Training_02_exe)

# ############################# CUDA Kernel ##############################
add_library(
    ${LIB_PROJECT_NAME}
    STATIC
    "functional.cu"
    ${PROJECT_INCLUDE_DIR}/02/functional.cuh
)

target_include_directories(
    ${LIB_PROJECT_NAME}
    PUBLIC
    ${PROJECT_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLAD_INCLUDE_DIR}
)

target_compile_options(${LIB_PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-gencode arch=compute_86,code=sm_86>)

target_compile_features(${LIB_PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(
    ${LIB_PROJECT_NAME}
    PROPERTIES
    CUDA_SEPARABLE_COMPILATION
    ON
)

# ############################# Executable C++ ##############################
add_executable(
    ${EXE_PROJECT_NAME}
    "main.cpp"
    ${IMGUI_SOURCE_FILES}
    $<TARGET_OBJECTS:${LIB_PROJECT_NAME}>
    $<TARGET_OBJECTS:Common>
)

target_include_directories(
    ${EXE_PROJECT_NAME}
    PUBLIC
    ${PROJECT_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLAD_INCLUDE_DIR}
    ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
)

target_link_libraries(
    ${EXE_PROJECT_NAME}
    glfw
    OpenGL::GL
    OpenGL::GLU
    $<TARGET_OBJECTS:glad>
)

if(UNIX)
    target_link_libraries(
        ${EXE_PROJECT_NAME}
        X11
    )
endif(UNIX)

target_compile_options(${EXE_PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-gencode arch=compute_86,code=sm_86>)

target_compile_features(${EXE_PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(
    ${EXE_PROJECT_NAME}
    PROPERTIES
    CUDA_SEPARABLE_COMPILATION
    ON
)

add_custom_command(
    TARGET ${EXE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EXE_PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/bin/
)
