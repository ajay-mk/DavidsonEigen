 # Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.11.0)

# Project's name
project(davidson_eigensolver)

# required modules
include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckIncludeFiles)
include(ExternalProject)
include(FetchContent)


set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 11)

# Compile with optimization by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Cmake modules/macros are in a subdirectory to keep this file cleaner
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

# Include function to Load MKL
include(${CMAKE_MODULE_PATH}/FindMKL.cmake)

# Find MKL
find_package(MKL)
if (MKL_FOUND)
  include_directories(${MKL_INCLUDE_DIRS})
  set(MKL_INCLUDE ${MKL_INCLUDE_DIRS}/intel64/lp64)
  set(LINEAR_ALGEBRA ${MKL_LIBRARIES} pthread m dl)
  message(STATUS "Using MKL for the linear algebra calculations!")
  message(STATUS "MKL_INCLUDE_DIRS: " ${MKL_INCLUDE_DIRS})
  message(STATUS "MKL_LIBRARIES: " ${MKL_LIBRARIES})
else()
  # search for lapack
  find_package(LAPACK COMPONENTS Fortran REQUIRED)
  # Search for Blas
  find_package(BLAS COMPONENTS Fortran REQUIRED)
  set(LINEAR_ALGEBRA ${LAPACK_LIBRARIES} ${BLAS_LIBRARIES})
  message(STATUS "MKL NOT FOUND! Using default BLAS and LAPACK")

  # Find OpenMP
  find_package(OpenMP REQUIRED)

endif(MKL_FOUND)

# search for Eigen
find_package (Eigen3 REQUIRED NO_MODULE)
include_directories(${EIGEN3_INCLUDE_DIR})
message(STATUS "EIGEN Include: " ${EIGEN3_INCLUDE_DIR})

# Declare the cxxopts dependency
FetchContent_Declare(
       cxxopts
       GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
       GIT_TAG master
)
FetchContent_GetProperties(cxxopts)
if(NOT cxxopts_POPULATED)
 FetchContent_Populate(cxxopts)
 add_subdirectory(${cxxopts_SOURCE_DIR} ${cxxopts_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
include_directories(${cxxopts_SOURCE_DIR}/include)

# search boost
find_package(Boost 1.57.0 REQUIRED COMPONENTS program_options filesystem system timer unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})
set (BOOST_CFLAGS_PKG "-I${Boost_INCLUDE_DIRS}")
set(BOOST_LIBS_PKG "-L${Boost_LIBRARY_DIRS}")
foreach(_blib ${Boost_LIBRARIES})
  string(REGEX REPLACE ".*/lib([^/]*)\\.[^.]*$" "-l\\1" _blib ${_blib})
  set(BOOST_LIBS_PKG "${BOOST_LIBS_PKG} ${_blib}")
endforeach(_blib)

add_subdirectory(src)
#add_subdirectory(test)
