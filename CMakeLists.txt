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
	src/render_texture.c
	ev_lua.o
	lua_app.o
	lua_body.o
	lua_anim.o
	lua_particle.o
	lua_render_texture.o
	lua_sbatch.o
	lua_sprite.o
	lua_stage.o
	lua_texture.o
	lua_world.o
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
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
		set (SDL_MAIN "c:/SDL2/lib/x86/SDL2main.lib")
else()
	set( SDL_MAIN "SDLmain")
endif()

if("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pagezero_size 10000 -image_base 100000000")
endif()


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

add_library( ev SHARED
	${SOURCES} )


target_link_libraries( ev
	${SDL2_LIBRARY}
	${SDL2_IMAGE_LIBRARY}
	${OPENGL_LIBRARY}
	Box2D
	jansson
	${LUA_LIBRARY}
	${SDL2_MAIN}
)

add_executable( ev_lua
	src/main.c
	)

target_link_libraries( ev_lua ev)

add_custom_command(
	OUTPUT ev_lua.o
	COMMAND luajit -b "${PROJECT_SOURCE_DIR}/lib/ev.lua" ev_lua.o
	DEPENDS lib/ev.lua
	)
add_custom_command(
	OUTPUT lua_app.o
	COMMAND luajit -b "${PROJECT_SOURCE_DIR}/lib/app.lua" lua_app.o
	DEPENDS lib/app.lua
	)
add_custom_command(
	OUTPUT lua_body.o
	COMMAND luajit -b "${PROJECT_SOURCE_DIR}/lib/body.lua" lua_body.o
	DEPENDS lib/body.lua
	)
add_custom_command(
	OUTPUT lua_particle.o
	COMMAND luajit -b "${PROJECT_SOURCE_DIR}/lib/particle.lua" lua_particle.o
	DEPENDS lib/particle.lua
	)
add_custom_command(
	OUTPUT lua_render_texture.o
	COMMAND luajit -b "${PROJECT_SOURCE_DIR}/lib/render_texture.lua" lua_render_texture.o
	DEPENDS lib/render_texture.lua
	)
add_custom_command(
	OUTPUT lua_sbatch.o
	COMMAND luajit -b "${PROJECT_SOURCE_DIR}/lib/sbatch.lua" lua_sbatch.o
	DEPENDS lib/sbatch.lua
	)
add_custom_command(
	OUTPUT lua_sprite.o
	COMMAND luajit -b "${PROJECT_SOURCE_DIR}/lib/sprite.lua" lua_sprite.o
	DEPENDS lib/sprite.lua
	)

add_custom_command(
	OUTPUT lua_stage.o
	COMMAND luajit -b "${PROJECT_SOURCE_DIR}/lib/stage.lua" lua_stage.o
	DEPENDS lib/stage.lua
	)
add_custom_command(
	OUTPUT lua_texture.o
	COMMAND luajit -b "${PROJECT_SOURCE_DIR}/lib/texture.lua" lua_texture.o
	DEPENDS lib/texture.lua
	)

add_custom_command(
	OUTPUT lua_world.o
	COMMAND luajit -b "${PROJECT_SOURCE_DIR}/lib/world.lua" lua_world.o
	DEPENDS lib/world.lua
	)
add_custom_command(
	OUTPUT lua_anim.o
	COMMAND luajit -b "${PROJECT_SOURCE_DIR}/lib/anim.lua" lua_anim.o
	DEPENDS lib/anim.lua
	)
