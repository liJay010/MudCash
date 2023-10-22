# 定义参与编译的源代码文件 
set(SRC_LIST testjson.cpp)
# 编译生成动态库mymuduo
add_executable(test_json ${SRC_LIST})
