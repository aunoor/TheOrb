cmake_minimum_required(VERSION 3.1)

project(TheOrbGatherer)

set(OG_BIN_DIR "bin" CACHE STRING "Binaries directory")
set(OG_INCLUDE_DIR "include" CACHE STRING "Includes directory")
set(OG_LIB_DIR "lib" CACHE STRING "Libraries directory")
set(OG_CONFIG_DIR "share/cmake" CACHE STRING "CMake config directory")

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/share/cmakemodules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/share/cmakemodules" )
list(APPEND CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/CMake")

add_subdirectory(sources)
