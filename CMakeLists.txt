cmake_minimum_required(VERSION 3.0)
project(MySkipList)

#头文件位置
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/*.h)

#输出文件名字
set(MAIN_TEST main)
set(STRESS_TEST stress)

#源文件位置
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

#头文件位置
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#测试文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(${MAIN_TEST} ${SRC})

#添加子文件(子文件写在最后)
add_subdirectory(stressTest)