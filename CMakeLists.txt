cmake_minimum_required(VERSION 3.1.0)
project(Zombie VERSION 0.1.0)
set(OpenGL_GL_PREFERENCE "GLVND")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

# Includes and libraries
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(OpenGL)

# subdirectories
add_subdirectory(JEngine)
add_subdirectory(graphicsStuff)

file( GLOB game_SOURCES *.cpp )

# Executable and link
add_executable(GameApp ${game_SOURCES})
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(GameApp  JEngine graphicsStuff ${SDL2_LIBRARIES} SDL2_mixer SDL2_ttf ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ) 


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
