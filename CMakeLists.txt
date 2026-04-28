cmake_minimum_required(VERSION 3.22)
project(mylog VERSION 1.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)   #如果当前的编译器太老，不支持 C++11，CMake 就会在生成阶段直接报错，而不是默默降级。

# 4. 开启额外的编译器警告（推荐养成好习惯）
add_compile_options(-Wall -Wextra -g)

# 自动帮找 pthread
find_package(Threads REQUIRED)

set(base_SRCS
    Logger.cpp
    LogStream.cpp
)

# 6. 定义要生成的可执行文件，以及它的源文件
add_executable(mylog main.cpp ${base_SRCS})
# 2. 不要写死 pthread，而是链接 CMake 提供的一个虚拟目标 Threads::Threads
target_link_libraries(mylog Threads::Threads)