cmake_minimum_required(VERSION 3.15)
project(CGA)

# Configure CMake
set(CMAKE_CXX_STANDARD 20)

# Install Conan packages
if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/cmake/conan.cmake")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
            "${CMAKE_CURRENT_LIST_DIR}/cmake/conan.cmake"
            TLS_VERIFY ON)
endif()
include("${CMAKE_CURRENT_LIST_DIR}/cmake/conan.cmake")
conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_CURRENT_LIST_DIR})

# Find Conan packages
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
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(CGA ${SOURCES})
target_include_directories(CGA PUBLIC include)
target_link_libraries(CGA ${PACKAGES})

# Copy resources
file(GLOB_RECURSE RESOURCES CONFIGURE_DEPENDS "res/*.*")
foreach(RESOURCE_PATH ${RESOURCES})
    file(RELATIVE_PATH RESOURCE_NAME "${CMAKE_CURRENT_LIST_DIR}/res" ${RESOURCE_PATH})
    configure_file(${RESOURCE_PATH} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${RESOURCE_NAME}" COPYONLY)
endforeach()
