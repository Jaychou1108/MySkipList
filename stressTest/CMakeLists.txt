add_executable(${STRESS_TEST} stress_test.cpp)

link_libraries(${STRESS_TEST} pthread)