cmake_minimum_required(VERSION 2.8)
Project ( ev )
SET( ev_SOURCE_DIR src)
enable_testing()

set(
  SOURCES
  src/main.c
  src/application.c
  src/application.h
  src/sprite.c
  src/sprite.h
  src/sprite_batch.c
  src/sprite_batch.h
  src/texture.c
  src/texture.h
  src/animation.c
  src/animation.h
  src/shader.c
  src/shader.h
  src/vector2.c
  src/vector2.h
  src/matrix4.c
  src/matrix4.h
  src/tile_map.h
  src/tile_map.c
  src/sprite_sheet.c
  src/sprite_sheet.h
  src/vertex_buffer.c
  src/vertex_buffer.h
  src/utils.c
  src/utils.h
  src/tile_map.c
  src/tile_map.h
  src/lua.c
  src/font.c
  src/font.h
  src/ev_lua.h
  src/ev_box2d.cpp
  src/ev_box2d.h
  src/debug_draw.cpp
  src/debug_draw.h
  src/render_texture.c
  src/render_texture.h
  src/graphics.c
  src/graphics.h
  src/quad_tree.c
  src/quad_tree.h
  src/rect.c
  src/rect.h
  src/array_list.h
  src/array_list.c
  ev_lua.o
  lua_app.o
  lua_body.o
  lua_anim.o
  lua_particle.o
  lua_render_texture.o
  lua_sbatch.o
  lua_sprite.o
  lua_texture.o
  lua_world.o
  lua_graphics.o
  lua_wait.o
  lua_vec2.o
  lua_matrix4.o
  lua_font.o
  lua_utils.o
  lua_tween.o
  lua_list.o
  lib/ev.lua
  lib/anim.lua
  lib/app.lua
  lib/particle.lua
  lib/render_texture.lua
  lib/sbatch.lua
  lib/sprite.lua
  lib/texture.lua
  lib/world.lua
  lib/body.lua
  lib/graphics.lua
  lib/wait.lua
  lib/vec2.lua
  lib/matrix4.lua
  lib/font.lua
  lib/utils.lua
  lib/tween.lua
  lib/list.lua
  vendor/glew/src/glew.c
)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_GENERATOR STREQUAL "Ninja")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics -Wall")
endif()

set(CMAKE_C_FLAGS_RELEASE, "#{CMAKE_C_FLAGS_RELEASE -O3}")


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
#set(GLEW_ROOT_DIR "${CMAKE_SOURCE_DIR}/vendor/glew")
set(freetype-gl_BUILD_DEMOS 0)
add_subdirectory( vendor/jansson)
add_subdirectory( vendor/Box2D )
add_subdirectory( vendor/freetype-gl)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(LuaJIT REQUIRED)
find_package(Freetype REQUIRED)

if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux" )
  set( SDL_MAIN "SDL2main")
  set(CMAKE_C_FLAGS_DEBUG, "#{CMAKE_C_FLAGS_DEBUG} -g")

elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
  #ugh
  set (SDL2_MAIN "c:/SDL2/lib/x86/SDL2main.lib")
else()
  set(CMAKE_C_FLAGS_DEBUG, "#{CMAKE_C_FLAGS_DEBUG} -g")
  set( SDL_MAIN "SDLmain")
endif()

if("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pagezero_size 10000 -image_base 100000000")
endif()

SET(EV_INCLUDES
  ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIRS}
  ${jansson_BINARY_DIR}/include
  "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew/include"
  ${LUA_INCLUDE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/vendor/uthash"
  "${CMAKE_CURRENT_SOURCE_DIR}/vendor"
  "${CMAKE_CURRENT_SOURCE_DIR}/src" PARENT_SCOPE)

MESSAGE("INCLUDES ${EV_INCLUDES}")

include_directories(
  ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIRS}
  ${jansson_BINARY_DIR}/include
  "${CMAKE_CURRENT_SOURCE_DIR}/vendor/freetype-gl"
  "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew/include"
  ${LUA_INCLUDE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/vendor/uthash"
  "${CMAKE_CURRENT_SOURCE_DIR}/vendor"
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  )
add_definitions(
  -D_CRT_SECURE_NO_WARNINGS
  -DGLEW_STATIC
  )

MESSAGE(${LUA_INCLUDE_DIR})
add_executable( ev_lua
  ${SOURCES}
  )

add_library(ev STATIC
  ${SOURCES}
)

target_link_libraries( ev_lua
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARY}
  ${OPENGL_LIBRARY}
  ${FREETYPE_LIBRARY}
  freetype-gl
  Box2D
  jansson
  ${LUA_LIBRARY}
  ${SDL2_MAIN}
  )

target_link_libraries(ev
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARY}
  Box2D
  jansson
  )

add_subdirectory(test)

# this should be cleaner somehow
add_custom_command(
  OUTPUT ev_lua.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/ev.lua" ev_lua.o
  DEPENDS lib/ev.lua
  )
add_custom_command(
  OUTPUT lua_app.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/app.lua" lua_app.o
  DEPENDS lib/app.lua
  )
add_custom_command(
  OUTPUT lua_body.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/body.lua" lua_body.o
  DEPENDS lib/body.lua
  )
add_custom_command(
  OUTPUT lua_particle.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/particle.lua" lua_particle.o
  DEPENDS lib/particle.lua
  )
add_custom_command(
  OUTPUT lua_render_texture.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/render_texture.lua" lua_render_texture.o
  DEPENDS lib/render_texture.lua
  )
add_custom_command(
  OUTPUT lua_sbatch.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/sbatch.lua" lua_sbatch.o
  DEPENDS lib/sbatch.lua
  )
add_custom_command(
  OUTPUT lua_sprite.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/sprite.lua" lua_sprite.o
  DEPENDS lib/sprite.lua
  )

add_custom_command(
  OUTPUT lua_texture.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/texture.lua" lua_texture.o
  DEPENDS lib/texture.lua
  )

add_custom_command(
  OUTPUT lua_world.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/world.lua" lua_world.o
  DEPENDS lib/world.lua
  )
add_custom_command(
  OUTPUT lua_anim.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/anim.lua" lua_anim.o
  DEPENDS lib/anim.lua
  )
add_custom_command(
  OUTPUT lua_graphics.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/graphics.lua" lua_graphics.o
  DEPENDS lib/graphics.lua
  )
add_custom_command(
  OUTPUT lua_wait.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/wait.lua" lua_wait.o
  DEPENDS lib/wait.lua
  )
add_custom_command(
  OUTPUT lua_vec2.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/vec2.lua" lua_vec2.o
  DEPENDS lib/vec2.lua
  )
add_custom_command(
  OUTPUT lua_matrix4.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/matrix4.lua" lua_matrix4.o
  DEPENDS lib/matrix4.lua
  )

add_custom_command(
  OUTPUT lua_font.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/font.lua" lua_font.o
  DEPENDS lib/font.lua
  )

add_custom_command(
  OUTPUT lua_utils.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/utils.lua" lua_utils.o
  DEPENDS lib/utils.lua
  )

add_custom_command(
  OUTPUT lua_tween.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/tween.lua" lua_tween.o
  DEPENDS lib/tween.lua
  )
add_custom_command(
  OUTPUT lua_list.o
  COMMAND ${LUAJIT_BINARY} -bg "${PROJECT_SOURCE_DIR}/lib/list.lua" lua_list.o
  DEPENDS lib/list.lua
  )
