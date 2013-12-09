cmake_minimum_required(VERSION 2.8)
Project ( Cavebound )
set(
  SOURCES
	src/main.cpp
)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")

ADD_DEFINITIONS(
  -std=c++11
)
find_package(OpenGL)
find_package(SDL2)
link_libraries(
  ${SDL2_LIBRARY}
	${SDL2IMAGE_LIBRARY}
	${OPENGL_LIBRARY}
	SDLmain
)

add_executable(
	cavebound
	${SOURCES}
)

