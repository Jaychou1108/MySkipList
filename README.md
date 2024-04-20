# KV存储项目
本项目是基于C++实现的一个轻量的键值对数据库，实现了数据插入、数据删除、数据查找、数据展示、数据落盘、数据库大小等功能。

在随机读写下，每秒可处理写请求数27.3w，每秒可处理读请求数31.5w

# 项目中文件
* bin:输出的可执行文件
* build:编译目录
* store:数据落盘文件
* stressTest:压力测试源代码
* CMakeLists.txt:CMake构建文件
* SkipList.h:跳表源代码文件


# 项目使用
1. 创建build目录，mkdir build , 创建文件落盘目录 mkdir store
2. 进入build文件
3. 终端中输入 cmake..
4. 接着输入 make
5. 可执行文件在bin中，执行./main(./stress)

