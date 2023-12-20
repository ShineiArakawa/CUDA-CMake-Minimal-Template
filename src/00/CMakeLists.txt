project(CUDA_Training_00 LANGUAGES CUDA CXX)

set(LIB_PROJECT_NAME CUDA_Training_00_kernel)
set(EXE_PROJECT_NAME CUDA_Training_00_exe)

############################## CUDA Kernel ##############################
add_library(
    ${LIB_PROJECT_NAME}
    STATIC
    "functional.cu"
    ${PROJECT_INCLUDE_DIR}/01/functional.cuh
)

target_include_directories(
    ${LIB_PROJECT_NAME}
    PUBLIC
    ${PROJECT_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${GLAD_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
)

target_compile_options(${LIB_PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-gencode arch=compute_86,code=sm_86>)
    
target_compile_features(${LIB_PROJECT_NAME} PUBLIC cxx_std_17)
    
set_target_properties(
    ${LIB_PROJECT_NAME}
    PROPERTIES
    CUDA_SEPARABLE_COMPILATION
    ON
)

############################## Executable C++ ##############################
add_executable(
    ${EXE_PROJECT_NAME}
    "main.cpp"
    ${IMGUI_SOURCE_FILES}
    $<TARGET_OBJECTS:${LIB_PROJECT_NAME}>
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
    ${GLFW_LIB_DIR}/libglfw3.a
    X11
    ${LIB_GLAD}
)

target_compile_options(${EXE_PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-gencode arch=compute_86,code=sm_86>)

target_compile_features(${EXE_PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(
    ${EXE_PROJECT_NAME}
    PROPERTIES
    CUDA_SEPARABLE_COMPILATION
    ON
)

