cmake_minimum_required(VERSION 2.8)
Project ( Cavebound )
set(
  SOURCES
	src/main.cpp
)

Find_Package( SDL REQUIRED )
Find_Package( SDL_image REQUIRED )
find_package(OpenGL)
link_libraries(
  ${SDL_LIBRARY}
	${SDLIMAGE_LIBRARY}
	SDLmain
)

add_executable(
	cavebound
	${SOURCES}
)

