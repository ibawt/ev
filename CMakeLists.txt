cmake_minimum_required(VERSION 2.8)
Project ( Cavebound )
set(
  SOURCES
	src/main.cpp
	src/application.cpp
	src/sprite.cpp
	src/sprite_batch.cpp
	src/texture.cpp
)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
add_subdirectory( gtest )
add_subdirectory( vendor/jansson)

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include
	${gtest_SOURCE_DIR}
	${SDL2_INCLUDE}
	${jansson_BINARY_DIR}/include
	src)
# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -Wall")
else ()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()

find_package(OpenGL)
find_package(SDL2)

add_library( evil STATIC
	${SOURCES} )

target_link_libraries( evil
  ${SDL2_LIBRARY}
	${SDL2IMAGE_LIBRARY}
	${OPENGL_LIBRARY}
	jansson
	SDL2main
)

add_executable( test_app
	test_app/test_app.cpp
)

target_link_libraries( test_app evil)
add_executable(
  runUnitTests
	test/testgtest.cpp)
target_link_libraries(runUnitTests gtest gtest_main evil ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${OPENGL_LIBRARY})
add_test(runUnitTests runUnitTests)
