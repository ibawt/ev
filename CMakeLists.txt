cmake_minimum_required(VERSION 2.8)
Project ( Cavebound )
set(
  SOURCES
	src/main.cpp
)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
add_subdirectory( gtest )
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${SDL2_INCLUDE})
ADD_DEFINITIONS(
  -std=c++11
)
find_package(OpenGL)
find_package(SDL2)

add_executable(
	cavebound
	${SOURCES}
)

target_link_libraries( cavebound
  ${SDL2_LIBRARY}
	${SDL2IMAGE_LIBRARY}
	${OPENGL_LIBRARY}
	SDLmain
)

add_executable(
  runUnitTests
	test/testgtest.cpp)
target_link_libraries(runUnitTests gtest gtest_main)
add_test(runUnitTests runUnitTests)
