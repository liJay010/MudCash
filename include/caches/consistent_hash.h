#pragma once
#include "string"
#include "SkipList.h"
#include "unordered_map"
#include "unordered_set"
#include "vector"
#include "iostream"

class consistent_hash {
public:
    unsigned int real_node_sum; // 真实节点数量
    unsigned int virtual_node_sum; //虚拟节点数量
    std::unordered_map<std::string, std::unordered_set<unsigned int>> real_node_map; //真实节点对应的虚拟节点的hash值
    std::unique_ptr<SkipList<unsigned int, std::string>> consistent_hash_list;  // 哈希环
    consistent_hash();
    ~consistent_hash();
    std::string search_by_name(std::string name);
    //添加真实节点 -- 第二个参数为虚拟节点个数
    void add_real_node(std::string ip, unsigned int virtual_node_num);
    //删除真实节点
    void drop_real_node(std::string ip);
    //打印所有节点
    void print();
};

