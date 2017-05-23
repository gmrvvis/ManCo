cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(manco VERSION 1.0.0 )
set(manco_VERSION_ABI 1)

# WAR for AppleClang bug
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
	
  set(CMAKE_CXX_FLAGS "-D'_LIBCPP_EXTERN_TEMPLATE(...)='")
endif()

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

set(MANCO_DESCRIPTION "ManCo (MANager COntainer)")
set(COMMON_PROJECT_DOMAIN Cristian Rodríguez Bernal)

include(Common)

# Allow warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

common_find_package(Lexis REQUIRED)
common_find_package(ZeroEQ REQUIRED)
common_find_package(gmrvlex REQUIRED)

list(APPEND MANCO_DEPENDENT_LIBRARIES 
  Lexis 
  ZeroEQ 
  gmrvlex
)

common_find_package_post( )

add_subdirectory( manco )
add_subdirectory( examples )

set(DOXYGEN_MAINPAGE_MD ${PROJECT_SOURCE_DIR}/README.md)
set(DOXYGEN_EXTRA_INPUT ${DOXYGEN_MAINPAGE_MD})
include(CPackConfig)
include(DoxygenRule)
