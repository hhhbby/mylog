# 创建测试可执行文件，比如叫 run_tests
add_executable(logfiletest LogFile_test.cpp)

# 将外层编译的 mylog_lib 链接到这个可执行文件中
target_link_libraries(logfiletest PRIVATE mylog_lib)


add_executable(appendfiletest AppendFile_test.cpp)
target_link_libraries(appendfiletest PRIVATE mylog_lib)