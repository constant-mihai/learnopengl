# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(learnogl LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_TOOLCHAIN_FILE /store/apps/vcpkg/scripts/buildsystems/vcpkg.cmake)
#set(CMAKE_PREFIX_PATH /store/apps/vcpkg/installed/x64-linux/share/assimp/assimp-config-version.cmake)
link_directories(./vcpkg/installed/x64-linux/lib)

include_directories(includes
    /usr/include/GL
	/usr/include/glm
	/usr/include/GLFW
	/store/code/cpp/stb
	/store/code/cpp/tinyobjloader
	/store/code/cpp/assimp/include)
find_package(assimp CONFIG REQUIRED)
#target_link_libraries(main PRIVATE assimp::assimp)

add_executable(logl main.cpp)

target_link_libraries(logl PRIVATE glfw GL GLEW assimp)
