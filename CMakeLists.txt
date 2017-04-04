cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(csb1 VERSION 1.0.0 )
set(csb1_VERSION_ABI 1)

# Disable in source building
if( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
  message( FATAL "no in source building allowed." )
endif()

list(APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common)

# Default build type is Debug
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug )
endif( NOT CMAKE_BUILD_TYPE )

if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
  add_definitions( -DDEBUG )
endif( )

include(GitExternal)

set(CSB1_DESCRIPTION "CSB1Plus (Simple Event Handler ++)")
set(COMMON_PROJECT_DOMAIN maldicion069)

include(Common)

# Allow warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")


common_find_package(Qt5Core SYSTEM REQUIRED)
common_find_package(Lexis REQUIRED)
common_find_package(ZeroEQ REQUIRED)
common_find_package(gmrvlex REQUIRED)

list(APPEND CSB1_DEPENDENT_LIBRARIES 
  Qt5Core

  Lexis 
  ZeroEQ 
  gmrvlex
)


common_find_package_post( )

add_subdirectory( csb1 )
add_subdirectory( examples )

set(DOXYGEN_MAINPAGE_MD ${PROJECT_SOURCE_DIR}/README.md)
set(DOXYGEN_EXTRA_INPUT ${DOXYGEN_MAINPAGE_MD})
include(CPackConfig)
include(DoxygenRule)
