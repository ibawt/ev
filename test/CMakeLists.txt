include_directories( ../vendor/cmocka ${EV_INCLUDES})

add_library( cmocka ../vendor/cmocka/cmocka.c)

add_executable(test_matrix4 matrix4_test.c)
target_link_libraries(test_matrix4 cmocka)

add_test(matrix4test test_matrix4)

add_executable(test_animation animation_test.c)
target_link_libraries(test_animation cmocka ${LUA_LIBRARY})
add_test(animationtest test_animation)

add_executable(test_qtree qtree_test.c)
target_link_libraries(test_qtree cmocka ev)
add_test(qtreetest test_qtree)
