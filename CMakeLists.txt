cmake_minimum_required(VERSION 2.8)
Project ( evil )

SET( evil_SOURCE_DIR src)
set(
  SOURCES
  src/main.cpp
  src/application.cpp
  src/sprite.cpp
  src/sprite_batch.cpp
  src/texture.cpp
  src/animation.cpp
  src/shader.cpp
  src/matrix4.cpp
  vendor/glew/src/glew.c
)
SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
add_subdirectory( gtest )
add_subdirectory( vendor/jansson)
add_subdirectory( vendor/Box2D )
enable_testing()

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
  src)
# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")

    endif ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -Wall -g")
else ()
    #message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()
add_definitions(
  -DGLEW_STATIC
 )

add_library( evil STATIC
  ${SOURCES} )

target_link_libraries( evil
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARY}
  ${OPENGL_LIBRARY}
  Box2D
  jansson
  ${SDL_MAIN}
)

add_executable( test_app
  test_app/test_app.cpp
)

target_link_libraries( test_app evil)
add_executable(
  runUnitTests
  test/testgtest.cpp
  test/animation_test.cpp
  )
target_link_libraries(runUnitTests gtest gtest_main evil ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${OPENGL_LIBRARY})
add_test(runUnitTests runUnitTests)
