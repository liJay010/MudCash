# 定义参与编译的源代码文件 
set(SRC_LIST testserver.cc)
# 编译生成动态库mymuduo
add_executable(test_server ${SRC_LIST})
target_link_libraries(test_server net pthread)
