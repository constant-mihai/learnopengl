# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(learnogl LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
#link_directories(/store/apps/vcpkg/installed/x64-linux/debug/lib)
#link_directories(/store/code/assimp/installed/x64-linux/lib)

file(GLOB_RECURSE commonogl_sourcelist "common/*.cpp" "common/*.hpp")
add_library(commonogl
    STATIC
    ${commonogl_sourcelist})

include_directories(includes
    /usr/include/GL
	/usr/include/glm
	/usr/include/GLFW
	/usr/include/GLFW
	/usr/include/assimp
	/store/code/cpp/stb
	/store/code/cpp/tinyobjloader
	/store/code/cpp/assimp/include)

find_package(assimp CONFIG REQUIRED)
#target_link_libraries(main PRIVATE assimp::assimp)

add_executable(logl main.cpp)

target_link_libraries(logl PRIVATE glfw GL GLEW assimp commonogl)
