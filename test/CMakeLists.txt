include_directories( ../vendor/cmocka)

add_library( cmocka ../vendor/cmocka/cmocka.c)

add_executable(test_matrix4 matrix4_test.c)
target_link_libraries(test_matrix4 cmocka)

add_test(matrix4test test_matrix4)

add_executable(test_animation animation_test.c)
target_link_libraries(test_animation cmocka ${LUA_LIBRARY})
add_test(animationtest test_animation)