cmake_minimum_required(VERSION 3.14)

# set(CMAKE_C_COMPILER "Compiler here")
# set(CMAKE_CXX_COMPILER "Compiler here")

project(LearnOpenAL) # add 'LANGUAGES C CXX', optional

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
  external/glad/src/glad.c
  scripts/openALinit.cpp
)

target_include_directories(${PROJECT_NAME}
  external/openal-soft/include # isn't included inside this folder so you must add it yourself
  external/glad/include # same thing for this library
  external/stb_vorbis # you get the point
  external/glm
  external
)

target_link_directories(${PROJECT_NAME}
  PRIVATE
  glfw
  OpenGL::GL
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sound DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
