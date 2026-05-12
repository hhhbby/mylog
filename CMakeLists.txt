cmake_minimum_required(VERSION 3.22)
project(mylog VERSION 1.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)   #如果当前的编译器太老，不支持 C++11，CMake 就会在生成阶段直接报错，而不是默默降级。

# 4. 开启额外的编译器警告（推荐养成好习惯）
add_compile_options(-Wall -Wextra -g)

# 自动帮找 pthread
#find_package(Threads REQUIRED)

set(base_SRCS
    Logger.cpp
    LogStream.cpp
    LoggFile.cpp
)

file(GLOB MYLOG_SOURCES "*.cpp")
add_library(mylog_lib STATIC ${MYLOG_SOURCES})
# 设置头文件搜索路径，PUBLIC 表示链接了这个库的目标也能使用这个路径
target_include_directories(mylog_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 6. 定义要生成的可执行文件，以及它的源文件

# 2. 不要写死 pthread，而是链接 CMake 提供的一个虚拟目标 Threads::Threads
#target_link_libraries(mylog Threads::Threads)

add_subdirectory(tests)