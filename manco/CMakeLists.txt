source_group(\\ FILES CMakeLists.txt)

set( MANCO_PUBLIC_HEADERS
  ZeqManager.hpp
)

set( MANCO_HEADERS
)

set( MANCO_SOURCES
  ZeqManager.cpp
)

set( MANCO_LINK_LIBRARIES
  ZeroEQ
  Lexis
  gmrvlex
)

common_library( manco )
