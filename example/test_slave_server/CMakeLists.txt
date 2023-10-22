# 定义参与编译的源代码文件 
set(SRC_LIST test_slave_server.cc)
# 编译生成动态库mymuduo
add_executable(test_slave_server ${SRC_LIST})
target_link_libraries(test_slave_server caches net pthread)
