cmake_minimum_required(VERSION 3.8)
project(em2d_sfml LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Raylib instead of SFML
find_package(raylib CONFIG REQUIRED)

# Debug output
if (TARGET raylib)
    message(STATUS "Raylib found and available!")
else()
    message(FATAL_ERROR "Raylib not found")
endif()

find_package(nlohmann_json CONFIG QUIET)

# Collect sources from the parent src folder (since this CMakeLists.txt is in em2d_sfml/ subdirectory)
file(GLOB_RECURSE EM2D_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.c"
)

# Debug: print found sources
message(STATUS "Found sources: ${EM2D_SOURCES}")

# Build the application with Raylib
message(STATUS "Building EM2D application with Raylib")
add_executable(em2d ${EM2D_SOURCES})
target_include_directories(em2d PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

if (TARGET nlohmann_json::nlohmann_json)
    target_link_libraries(em2d PRIVATE nlohmann_json::nlohmann_json)
    message(STATUS "Using nlohmann_json")
else()
    message(WARNING "nlohmann_json not found - JSON config loading may fail")
endif()

# Link Raylib
target_link_libraries(em2d PRIVATE raylib)

if(MSVC)
    target_compile_options(em2d PRIVATE /W4 /permissive-)
else()
    target_compile_options(em2d PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install rule (optional)
install(TARGETS em2d RUNTIME DESTINATION bin)
