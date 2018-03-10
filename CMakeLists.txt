# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   GMRVLEX
#   2017 (c) GMRV / Universidad Rey Juan Carlos
#   gmrv@gmrv.es
#   www.gmrv.es
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
cmake_minimum_required( VERSION 3.1 FATAL_ERROR )
project( manco VERSION 1.0.0 )
set( PROJECT_VERSION_MAJOR "1" )
set( PROJECT_VERSION_MINOR "0" )
set( PROJECT_VERSION_PATCH "0" )
set( PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} )
set( manco_VERSION_ABI 1 )

# WAR for AppleClang bug
if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" )
  set( CMAKE_CXX_FLAGS "-D'_LIBCPP_EXTERN_TEMPLATE(...)='" )
endif( )

# Disable in source building
if( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
  message( FATAL "no in source building allowed." )
endif( )

list( APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common
)

# Default build type is Debug
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug )
endif( NOT CMAKE_BUILD_TYPE )

if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
  add_definitions( -DDEBUG )
endif( )

include( GitExternal )

set( MANCO_DESCRIPTION "ManCo (MANager COntainer)" )
set( COMMON_PROJECT_DOMAIN gmrv.es )

include( Common )
			
# Allow warnings
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w" )

common_find_package( Lexis REQUIRED )
common_find_package( ZeroEQ REQUIRED )
common_find_package( gmrvlex REQUIRED )

list( APPEND MANCO_DEPENDENT_LIBRARIES 
  Lexis 
  ZeroEQ 
  gmrvlex
)

common_find_package_post( )

add_subdirectory( manco )
add_subdirectory( examples )

if( MSVC )
  # Allow warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
  
  # Set manco as startup project on Visual Studio, CMake v3.6
  if( CMAKE_VERSION VERSION_GREATER 3.6 )
    set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT manco )
  endif( )

  
  # Excluded because they fail on compilation on win32 and CI
  set(EXCLUDED_FROM_BUILD
    "Lexis-test-render-histogram"
	"Lexis-test-data-progress"
	"Lexis-test-render-clipPlanes"
	"Lexis-test-render-imageJPEG"
	"vmmlib-test-aabb"
	"vmmlib-test-frustum"
	"vmmlib-test-matrix"
	"vmmlib-test-quaternion"
	"vmmlib-test-vector"
	"testschema"
  )
  set_target_properties(${EXCLUDED_FROM_BUILD} PROPERTIES EXCLUDE_FROM_ALL "TRUE")
endif( )

set( DOXYGEN_MAINPAGE_MD ${PROJECT_SOURCE_DIR}/README.md )
set( DOXYGEN_EXTRA_INPUT ${DOXYGEN_MAINPAGE_MD} )
include( CPackConfig )
include( DoxygenRule )
