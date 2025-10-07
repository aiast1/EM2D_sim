# CMakeList.txt : CMake project for EM2D_sim, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (EM2D_sim "EM2D_sim.cpp" "EM2D_sim.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET EM2D_sim PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
