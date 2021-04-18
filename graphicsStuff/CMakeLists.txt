file( GLOB graphicsStuff_SOURCES *.cpp )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../JEngine)
add_library( graphicsStuff ${graphicsStuff_SOURCES} )
target_include_directories(graphicsStuff PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

