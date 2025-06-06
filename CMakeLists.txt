cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(DH2323Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O2 -Wall")

IF(APPLE)
  SET(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Build architectures for Mac OS X" FORCE)
ENDIF(APPLE)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package (SDL2 REQUIRED)

message(STATUS "Lib: ${SDL2_LIBRARIES} , Include: ${SDL2_INCLUDE_DIRS}")

include_directories(
  ${SDL2_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/glm
  ${CMAKE_SOURCE_DIR}/SDL2Auxiliary
  ${CMAKE_SOURCE_DIR}/src
)

add_executable(DH2323ProjectSDL2
  ${CMAKE_SOURCE_DIR}/Drawer.cpp
  ${CMAKE_SOURCE_DIR}/src/AnimatedSphere.cpp 
  ${CMAKE_SOURCE_DIR}/src/Camera.cpp 
  ${CMAKE_SOURCE_DIR}/src/Light.cpp 
  ${CMAKE_SOURCE_DIR}/src/Material.cpp 
  ${CMAKE_SOURCE_DIR}/src/PathTracer.cpp 
  ${CMAKE_SOURCE_DIR}/src/Ray.cpp 
  ${CMAKE_SOURCE_DIR}/src/Sampling.cpp 
  ${CMAKE_SOURCE_DIR}/src/Scene.cpp 
  ${CMAKE_SOURCE_DIR}/src/Shape.cpp 
  ${CMAKE_SOURCE_DIR}/src/Sphere.cpp 
  ${CMAKE_SOURCE_DIR}/src/Triangle.cpp 
  ${CMAKE_SOURCE_DIR}/src/VectorMath.cpp 
  ${CMAKE_SOURCE_DIR}/SDL2Auxiliary/SDL2Auxiliary.cpp
)

target_link_libraries(DH2323ProjectSDL2
  ${SDL2_LIBRARIES}
)