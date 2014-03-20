cmake_minimum_required(VERSION 2.8)
Project ( ev )
SET( ev_SOURCE_DIR src)
set(
  SOURCES
  src/application.c
  src/sprite.c
  src/sprite_batch.c
  src/texture.c
  src/animation.c
  src/shader.c
  src/vector2.c
  src/matrix4.c
  src/sprite_sheet.c
  src/vertex_buffer.c
  src/utils.c
  src/tile_map.c
  src/lua.c
  src/stage.c
  src/ev_box2d.cpp
  src/debug_draw.cpp
  ev_lua_lib.c
  vendor/glew/src/glew.c
)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_GENERATOR STREQUAL "Ninja")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics -Wall")
endif()

set(CMAKE_C_FLAGS_RELEASE, "#{CMAKE_C_FLAGS_RELEASE -O3}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
add_subdirectory( vendor/jansson)
add_subdirectory( vendor/Box2D )

find_package(OpenGL)
find_package(SDL2)
find_package(SDL2_image)
find_package(LuaJIT)

if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux" )
  set( SDL_MAIN "SDL2main")
elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
    set (SDL_MAIN "c:/SDL2/lib/x86/SDL2main.lib")
else()
  set( SDL_MAIN "SDLmain")
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pagezero_size 10000 -image_base 100000000")


include_directories(${gtest_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}
  ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIRS}
  ${jansson_BINARY_DIR}/include
  vendor/glew/include
  ${LUA_INCLUDE_DIR}
  vendor/uthash
  vendor
  src)
add_definitions(
  -D_CRT_SECURE_NO_WARNINGS
  -DGLEW_STATIC
  -g
 )

add_library( ev STATIC
  ${SOURCES} )


target_link_libraries( ev
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARY}
  ${OPENGL_LIBRARY}
  Box2D
  jansson
  ${LUA_LIBRARY}
  ${SDL_MAIN}
)

add_executable( test_app
  test_app/test_app.c
)

add_executable( ev_lua
  src/main.c
  )

target_link_libraries( ev_lua ev)
target_link_libraries( test_app ev)

enable_testing()
add_subdirectory(test)

add_executable(embedfile tools/embedfile.c)

add_custom_command(
  OUTPUT ev_lua_lib.c
  COMMAND embedfile ../lib/ev.lua ev_lua_lib
  DEPENDS lib/ev.lua
  )