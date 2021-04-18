file( GLOB JEngine_SOURCES *.cpp )
add_library( JEngine ${JEngine_SOURCES} )
target_include_directories(JEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
