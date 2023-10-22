# 定义参与编译的源代码文件 
aux_source_directory(. SRC_LIST)
# 编译生成动态库mymuduo
add_executable(slaveserver ${SRC_LIST} ../client/caclient.cpp)
target_link_libraries(slaveserver net pthread)