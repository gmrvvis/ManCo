source_group(\\ FILES CMakeLists.txt)

set( MANCO_PUBLIC_HEADERS
  ZeqManager.h
)

set( MANCO_HEADERS
  Definitions.hpp
)

set( MANCO_SOURCES
  ZeqManager.cpp
)

set( MANCO_LINK_LIBRARIES
  ZeroEQ
  Lexis
  gmrvlex
)

if (MSVC)
  add_definitions("/W4")
endif()

common_library( manco )
