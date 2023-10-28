#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

template<typename K, typename V>
class KVdatabase
{
public:
    KVdatabase(){};
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
};

template<typename K, typename V>
bool KVdatabase<K,V>::check_K(K key)
{
    return KVdata.find(key) != KVdata.end();
}

template<typename K, typename V>
V KVdatabase<K,V>::find_K(K key)
{
    return KVdata[key];
}

template<typename K, typename V>
void KVdatabase<K,V>::insert_KV(K key,V value)
{
    
    KVdata.insert({key,value});
    //cout << key << value <<endl;
}

template<typename K, typename V>
void KVdatabase<K,V>::delete_K(K key)
{
    KVdata.erase(key);
}

template<typename K, typename V>
void KVdatabase<K,V>::update_KV(K key,V value)
{
    if(check_K(key)) KVdata[key] = value;
}
template<typename K, typename V>
int KVdatabase<K,V>::size()
{
    return KVdata.size();
}
template<typename K, typename V>
std::vector<std::pair<K,V>> KVdatabase<K,V>::get_all_data()
{
    std::vector<std::pair<K,V>> temp;
    for(auto x: KVdata)
    {
        temp.push_back(x);
    }
    return temp;
}
template<typename K, typename V>
void KVdatabase<K,V>::clear()
{
    KVdata.clear();
}

template<typename K, typename V>
void KVdatabase<K,V>::show()
{

    for(auto x: KVdata)
    {
        std::cout << x.first << ":" << x.second<< std::endl;
    }
}