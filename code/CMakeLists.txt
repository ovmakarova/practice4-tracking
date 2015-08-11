cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/")
include(../cmake/general.cmake)

# Google Test
include_directories(SYSTEM "3rdparty")
add_subdirectory("3rdparty")

clear_dependency()
add_local_dependency("gtest")

# Include subprojects
add_all_sub_dir("${CMAKE_SOURCE_DIR}" except "3rdparty")

find_package(OpenCV REQUIRED)
set(LIBRARY_DEPS ${OpenCV_LIBS})

# REPORT
message( STATUS "")
message( STATUS "General configuration for ${PROJECT_NAME}")
message( STATUS "======================================")
message( STATUS "")
message( STATUS "   Configuration:        ${CMAKE_BUILD_TYPE}")
message( STATUS "   OpenCV build path:    ${OpenCV_DIR}")
message( STATUS "")
