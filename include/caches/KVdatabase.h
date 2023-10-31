#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include "SkipList.h"
template<typename K, typename V>
class KVdatabase
{
public:
    KVdatabase();
    bool check_K(K); //确保是否在数据库中
    V find_K(K); //查询K 对应的V值
    void insert_KV(K,V); //插入K,V
    void delete_K(K); //删除K
    void update_KV(K,V); //更改KV，如果不在，则不更改
    int size();
    void clear();
    void show();
    std::vector<std::pair<K,V>> get_all_data();
private:
    std::unordered_map<K ,V> KVdata;
    SkipList<std::string,std::string> skipList;
};

template<typename K, typename V>
KVdatabase<K,V>::KVdatabase():skipList(18)
{

}

template<typename K, typename V>
bool KVdatabase<K,V>::check_K(K key)
{
    return skipList.find_element(key);
}

template<typename K, typename V>
V KVdatabase<K,V>::find_K(K key)
{
    return skipList.search_element(key);
}

template<typename K, typename V>
void KVdatabase<K,V>::insert_KV(K key,V value)
{
    
    skipList.insert_element(key,value);
    //cout << key << value <<endl;
}

template<typename K, typename V>
void KVdatabase<K,V>::delete_K(K key)
{
    skipList.delete_element(key);
}

template<typename K, typename V>
void KVdatabase<K,V>::update_KV(K key,V value)
{
    skipList.update_element(key,value);
}
template<typename K, typename V>
int KVdatabase<K,V>::size()
{
    return skipList.size();
}
template<typename K, typename V>
std::vector<std::pair<K,V>> KVdatabase<K,V>::get_all_data()
{
    return skipList.get_all_data();
}
template<typename K, typename V>
void KVdatabase<K,V>::clear()
{
    skipList.clear();
}

template<typename K, typename V>
void KVdatabase<K,V>::show()
{
    skipList.display_list();
}