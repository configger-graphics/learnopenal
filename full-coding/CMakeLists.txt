cmake_minimum_required(VERSION 3.14)

# set(CMAKE_C_COMPILER "Compiler here")
# set(CMAKE_CXX_COMPILER "Compiler here")

project(LearnOpenAL) # add 'LANGUAGES C CXX', optional

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)

add_executable(${PROJECT_NAME}
  external/glad/src/glad.c
  external/stb_vorbis.c
  scripts/openALinit.cpp
)

target_include_directories(${PROJECT_NAME}
  external/glad/include # all the information are in text inside external folder
  external/glm
  external
)

# If you have pacman (which is what I have on UCRT64 MSYS2) type these:
# pacman -S mingw-w64-ucrt-x86_64-openal # For MSYS2
# sudo pacman -S openal # For Arch/Manjaro Linux
# The equivalent if not mentioned can be searched up by google

target_link_directories(${PROJECT_NAME}
  PRIVATE
  OpenAL::OpenAL
  glfw
  OpenGL::GL
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sound DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
