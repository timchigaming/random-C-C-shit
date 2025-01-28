cmake_minimum_required(VERSION 3.31.3 FATAL_ERROR)
set(PROJECT_NAME SHEEESH)

project(${PROJECT_NAME})

set(CMAKE_CCX_STANDART 17)


add_executable(${PROJECT_NAME}
src/main.cpp
src/Renderer/ShaderProgram.cpp
src/Renderer/ShaderProgram.h)

target_include_directories(${PROJECT_NAME} PRIVATE external/glad/include)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS  OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)