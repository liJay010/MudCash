# 定义参与编译的源代码文件 
set(SRC_LIST test_coServer.cc)
# 编译生成动态库mymuduo
add_executable(test_coServer ${SRC_LIST})
target_link_libraries(test_coServer caches net pthread)
