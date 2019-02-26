# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   ManCo
#   2017-2019 (c) GMRV / Universidad Rey Juan Carlos
#   gmrv@gmrv.es
#   www.gmrv.es
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

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
  VishnuLex
)

if (MSVC)
  add_definitions("/W4")
endif()

common_library( ManCo )
