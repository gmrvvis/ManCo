# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   ManCo
#   2017-2019 (c) GMRV / Universidad Rey Juan Carlos
#   gmrv@gmrv.es
#   www.gmrv.es
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

include_directories(
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set( MANCOCONSUMER_HEADERS )
set( MANCOCONSUMER_SOURCES consumer.cpp )
set( MANCOCONSUMER_LINK_LIBRARIES
  manco
)
common_application( mancoConsumer )


set( MANCOPRODUCER_HEADERS )
set( MANCOPRODUCER_SOURCES producer.cpp )
set( MANCOPRODUCER_LINK_LIBRARIES
  manco
)
common_application( mancoProducer )
