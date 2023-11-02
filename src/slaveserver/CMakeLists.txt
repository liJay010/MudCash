# 定义参与编译的源代码文件 
#aux_source_directory(. SRC_LIST)
# 编译生成动态库mymuduo
add_executable(slaveserver slaveServer.cpp slaveService.cpp ../client/caclient.cpp test_slave_server.cc)
#add_executable(slaveserver1 slaveServer.cpp slaveService.cpp ../client/caclient.cpp test_slave_server1.cc)
#add_executable(slaveserver2 slaveServer.cpp slaveService.cpp ../client/caclient.cpp test_slave_server2.cc)
target_link_libraries(slaveserver net pthread)
#target_link_libraries(slaveserver1 net pthread)
#target_link_libraries(slaveserver2 net pthread)