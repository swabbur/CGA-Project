cmake_minimum_required(VERSION 3.17)
project(Legacy)

# Configure CMake
set(CMAKE_CXX_STANDARD 20)

# Load Conan packages
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
set(PACKAGES
        CONAN_PKG::opengl
        CONAN_PKG::glew
        CONAN_PKG::glfw
        CONAN_PKG::glm
        CONAN_PKG::stb
        CONAN_PKG::assimp)

# Create executable
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(Legacy ${SOURCES})
target_include_directories(Legacy PUBLIC include)
target_link_libraries(Legacy ${PACKAGES})

# Copy resource folders
file(GLOB MESHES "res/meshes/*.obj")
file(GLOB SHADERS "res/shaders/*.glsl")
file(GLOB TEXTURES "res/textures/*.png")

file(COPY ${MESHES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/meshes)
file(COPY ${SHADERS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)
file(COPY ${TEXTURES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/textures)
