source_group(\\ FILES CMakeLists.txt)

set( CSB1_ZEQ_PUBLIC_HEADERS
  ZeqManager.hpp
)

set( CSB1_ZEQ_HEADERS
)

set( CSB1_ZEQ_SOURCES
  ZeqManager.cpp
)

set( CSB1_ZEQ_LINK_LIBRARIES
  ZeroEQ
  Lexis
  gmrvlex

  Qt5::Core
)

common_library( csb1_zeq )
