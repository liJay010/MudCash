# 定义参与编译的源代码文件 
set(SRC_LIST main.cpp)
# 编译生成动态库mymuduo
add_executable(mudcache ${SRC_LIST})
target_link_libraries(mudcache caches net pthread)
