source_group(\\ FILES CMakeLists.txt)

set( CSB1_PUBLIC_HEADERS
  ZeqManager.hpp
)

set( CSB1_HEADERS
)

set( CSB1_SOURCES
  ZeqManager.cpp
)

set( CSB1_LINK_LIBRARIES
  ZeroEQ
  Lexis
  gmrvlex

  Qt5::Core
)

common_library( csb1 )
