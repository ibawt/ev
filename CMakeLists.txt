cmake_minimum_required(VERSION 2.8)
Project ( ev )
SET( ev_SOURCE_DIR src)
set(
  SOURCES
  src/application.c
  src/main.c
  src/sprite.c
  src/sprite_batch.c
  src/texture.c
  src/animation.c
  src/shader.c
  src/matrix4.c
  src/sprite_sheet.c
  src/vertex_buffer.c
  src/utils.c
  src/tile_map.c
  vendor/glew/src/glew.c
)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
add_subdirectory( vendor/jansson)
add_subdirectory( vendor/Box2D )
add_subdirectory( vendor/lua )

find_package(OpenGL)
find_package(SDL2)
find_package(SDL2_image)

if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux" )
  set( SDL_MAIN "SDL2main")
elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
    set (SDL_MAIN "c:/SDL2/lib/x86/SDL2main.lib")
else()
  set( SDL_MAIN "SDLmain")
endif()

include_directories(${gtest_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}
  ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIRS}
  ${jansson_BINARY_DIR}/include
  vendor/glew/include
  vendor/uthash
  vendor/lua/src
  ${CMAKE_CURRENT_BINARY_DIR}/vendor/lua
  src)
add_definitions(
  -DGLEW_STATIC
 )

add_library( ev STATIC
  ${SOURCES} )

target_link_libraries( ev
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARY}
  ${OPENGL_LIBRARY}
  Box2D
  jansson
  liblua
  ${SDL_MAIN}
)

add_executable( test_app
  test_app/test_app.c
)

target_link_libraries( test_app ev)
