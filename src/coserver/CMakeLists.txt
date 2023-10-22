# 定义参与编译的源代码文件 
aux_source_directory(. SRC_LIST)
# 编译生成动态库mymuduo
add_executable(coserver ${SRC_LIST} ../client/caclient.cpp ../hash/consistent_hash.cpp)
target_link_libraries(coserver net pthread)