# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.8)

# Force vcpkg toolchain usage 
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file" FORCE)
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "Vcpkg target triplet" FORCE)

project(EM2D_SimWorkspace LANGUAGES CXX)

# Add the em2d_sfml project (contains its own CMakeLists)
add_subdirectory(em2d_sfml)
