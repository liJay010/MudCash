#include "consistent_hash.h"

int HASH_LEN = 32;
const int RING_SIZE = 1e8;
const uint32_t CRC32_POLY = 0xEDB88320;
unsigned int CRC32(std::string s)
{   
	uint32_t crc = 0xFFFFFFFF;
    for (char c : s) {
        crc ^= static_cast<uint32_t>(c);
        for (int i = 0; i < 8; ++i) {
            crc = (crc >> 1) ^ (CRC32_POLY & (-(crc & 1)));
        }
    }
    return (~crc)% RING_SIZE;
}

unsigned int my_getMurMurHash(const void *key, int len) {
    const unsigned int m = 0x5bd1e995;
    const int r = 24;
    const int seed = 97;
    unsigned int h = seed ^len;
    // Mix 4 bytes at a time into the hash
    const unsigned char *data = (const unsigned char *) key;
    while (len >= 4) {
        unsigned int k = *(unsigned int *) data;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }
    // Handle the last few bytes of the input array
    switch (len) {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
    };
    // Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return h;
}


consistent_hash::consistent_hash()
{
    real_node_sum = 0; // 真实节点数量
    virtual_node_sum = 0; //虚拟节点数量
    this->consistent_hash_list = std::make_unique<SkipList<unsigned int, std::string>>(10);
}


std::string consistent_hash::search_by_name(std::string name)
{
    //unsigned int s = my_getMurMurHash(name.c_str(), HASH_LEN);
    unsigned int s = CRC32(name);
    std::string v_node = this->consistent_hash_list->search_element(s);

    return v_node;
}


//添加真实节点 -- 第二个参数为虚拟节点个数
void consistent_hash::add_real_node(std::string ip, unsigned int num)
{
    std::unordered_set<unsigned int> umap_hash;
    for(unsigned int i = 0; i < num ; i++ )
    {
        std::string ip_port = ip + std::to_string(i);
        //unsigned int hash_value = my_getMurMurHash(ip_port.c_str(), HASH_LEN);
        unsigned int hash_value = CRC32(ip_port);
        umap_hash.insert(hash_value);
        consistent_hash_list->insert_element(hash_value,ip);
    }
    real_node_map[ip] = umap_hash;
    real_node_sum++;
    this->virtual_node_sum+=num;
}


//删除真实节点
void consistent_hash::drop_real_node(std::string ip)
{
    unsigned int val_count = 0;
    if(real_node_map.find(ip) != real_node_map.end())
    {
        val_count = real_node_map[ip].size();
        for(unsigned int value: real_node_map[ip])
        {
            consistent_hash_list->delete_element(value);
        }
        real_node_map.erase(ip);
        real_node_sum--;
        virtual_node_sum-=val_count;
    }
}

//打印所有节点
void consistent_hash::print()
{
    std::cout << "True Node num: " << real_node_map.size() << std::endl;
    for(auto x: real_node_map)
    {
        std::cout << x.first <<"    The nums of virtual: " << x.second.size()<<std::endl;
    }
    std::cout << "All Node num: " << virtual_node_sum << std::endl;
    std::cout << "<<<<<<<<<<<<<<<<<<<<<<<end>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
}

consistent_hash::~consistent_hash()
{
    
}