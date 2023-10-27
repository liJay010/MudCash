#pragma once
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <fstream>
#include <cstdlib>
#include <memory>
#include <vector>
#include <utility>
#define STORE_FILE "store/dumpFile"

static std::mutex mtx;     // mutex for critical section
static std::string delimiter = ":";

//Class template to implement node
template<typename K, typename V>
class Node {

public:

    Node() {}

    Node(K k, V v, int);

    ~Node();

    K get_key() const;

    V get_value() const;

    void set_backup(bool);
    bool get_backup();

    void set_value(V);

    // Linear array to hold pointers to next node of different level
    std::vector<std::shared_ptr<Node<K, V>>> forward;
    //std::shared_ptr<Node<K, V>>*forward;

    int node_level;

private:
    bool backup; //该节点是否是备份数据 - 默认不是
    K key;
    V value;
};

template<typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level) {
    this->key = k;
    this->value = v;
    this->node_level = level;
    backup = false;
    // level + 1, because array index is from 0 - level
    //this->forward = new Node<K, V>*[level+1];
    this->forward.resize(level+1);

    // Fill forward array with 0(nullptr)
    //memset(this->forward, 0, sizeof(Node<K, V>*)*(level+1));
};

template<typename K, typename V>
Node<K, V>::~Node() {
    //delete []forward;
};

template<typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
};

template<typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
};
template<typename K, typename V>
void Node<K, V>::set_value(V value) {
    this->value=value;
};
template<typename K, typename V>
void Node<K, V>::set_backup(bool bk)
{
    this->backup=bk;
};

template<typename K, typename V>
bool Node<K, V>::get_backup()
{
    return this->backup;
}
// Class template for Skip list
template <typename K, typename V>
class SkipList {

public:
    SkipList(int);
    ~SkipList();
    int get_random_level();
    std::shared_ptr<Node<K, V>> create_node(K, V, int);
    int insert_element(K, V,bool backup = false);
    void display_list();

    std::vector<std::pair<K,V>> all_list(); //返回所有键值对
    //TODO
    //找到哈希环的上一个节点，下一个节点
    
    V search_element(K); //找K，找到返回，找不到返回下一个
    bool find_element(K,V*);//找K，找到返回0，V保存参数，找不到返回-1，

    void delete_element(K);
    void dump_file();
    void load_file();
    int size();

private:
    void get_key_value_from_string(const std::string& str, std::string key, std::string value);
    bool is_valid_string(const std::string& str);

private:
    // Maximum level of the skip list
    int _max_level;

    // current level of skip list
    int _skip_list_level;

    // pointer to header node
    //std::shared_ptr<Node<K, V>>_header;
    std::shared_ptr<Node<K, V>> _header;

    // file operator
    std::ofstream _file_writer;
    std::ifstream _file_reader;

    // skiplist current element count
    int _element_count;
};

// create new node
template<typename K, typename V>
std::shared_ptr<Node<K, V>> SkipList<K, V>::create_node(const K k, const V v, int level) {
    //std::shared_ptr<Node<K, V>>n = new Node<K, V>(k, v, level);
    return std::make_shared<Node<K, V>>(k, v, level);
}

// Insert given key and value in skip list
// return 1 means element exists
// return 0 means insert successfully
/*
                           +------------+
                           |  insert 50 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |                      insert +----+
level 3         1+-------->10+---------------> | 50 |          70       100
                                               |    |
                                               |    |
level 2         1          10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 1         1    4     10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 0         1    4   9 10         30   40  | 50 |  60      70       100
                                               +----+

*/
template<typename K, typename V>
int SkipList<K, V>::insert_element(K key, V value,bool backup) {

    mtx.lock();
    std::shared_ptr<Node<K, V>> current = this->_header;


    // create update array and initialize it
    // update is array which put node that the node->forward[i] should be operated later
    //std::shared_ptr<Node<K, V>>update[_max_level+1];
    std::vector<std::shared_ptr<Node<K, V>>> update(_max_level+1);
    //memset(update, 0, sizeof(Node<K, V>*)*(_max_level+1));

    // start form highest level of skip list
    for(int i = _skip_list_level; i >= 0; i--) {
        while(current->forward[i] != nullptr && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    // reached level 0 and forward pointer to right node, which is desired to insert key.
    current = current->forward[0];

    // if current node have key equal to searched key, we get it
    if (current != nullptr && current->get_key() == key) {
        current->set_value(value);
        current->set_backup(backup);
        mtx.unlock();
        return 1;
    }

    // if current is nullptr that means we have reached to end of the level
    // if current's key is not equal to key that means we have to insert node between update[0] and current node
    if (current == nullptr || current->get_key() != key ) {

        // Generate a random level for node
        int random_level = get_random_level();

        // If random level is greater thar skip list's current level, initialize update value with pointer to header
        if (random_level > _skip_list_level) {
            for (int i = _skip_list_level+1; i < random_level+1; i++) {
                update[i] = _header;
            }
            _skip_list_level = random_level;
        }

        // create new node with random level generated
        std::shared_ptr<Node<K, V>> inserted_node = create_node(key, value, random_level);
        inserted_node->set_backup(backup);
        // insert node
        for (int i = 0; i <= random_level; i++) {
            inserted_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = inserted_node;
        }
        _element_count ++;
    }
    mtx.unlock();
    return 0;
}

// Display skip list
template<typename K, typename V>
void SkipList<K, V>::display_list() {

    std::cout << "\n*****Skip List*****"<<"\n";
    for (int i = 0; i <= _skip_list_level; i++) {
        std::shared_ptr<Node<K, V>>node = this->_header->forward[i];
        std::cout << "Level " << i << ": ";
        while (node != nullptr) {
            std::cout << node->get_key() << ":" << node->get_value() << ";";
            node = node->forward[i];
        }
        std::cout << std::endl;
    }
}
template<typename K, typename V>
std::vector<std::pair<K,V>> SkipList<K, V>::all_list() //返回所有键值对
{
    std::vector<std::pair<K,V>> res;

    std::shared_ptr<Node<K, V>>node = this->_header->forward[0];

    while (node != nullptr) {
        std::cout << node->get_key() << ":" << node->get_value() << ";";
        res.push_back({node->get_key(),node->get_value()});
        node = node->forward[0];
    }
    return res;
}

// Dump data in memory to file
template<typename K, typename V>
void SkipList<K, V>::dump_file() {

    std::cout << "dump_file-----------------" << std::endl;
    _file_writer.open(STORE_FILE);
    std::shared_ptr<Node<K, V>> node = this->_header->forward[0];

    while (node != nullptr) {
        _file_writer << node->get_key() << ":" << node->get_value() << "\n";
        std::cout << node->get_key() << ":" << node->get_value() << ";\n";
        node = node->forward[0];
    }

    _file_writer.flush();
    _file_writer.close();
    return ;
}

// Load data from disk
template<typename K, typename V>
void SkipList<K, V>::load_file() {

    _file_reader.open(STORE_FILE);
    std::cout << "load_file-----------------" << std::endl;
    std::string line;
    std::string key;
    std::string value;
    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key.empty() || value.empty()) {
            continue;
        }
        insert_element(key, value,false);
        std::cout << "key:" << key << "value:" << value << std::endl;
    }
    _file_reader.close();
}

// Get current SkipList size
template<typename K, typename V>
int SkipList<K, V>::size() {
    return _element_count;
}

template<typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string& str, std::string key, std::string value) {

    if(!is_valid_string(str)) {
        return;
    }
    key = str.substr(0, str.find(delimiter));
    value = str.substr(str.find(delimiter)+1, str.length());
}

template<typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string& str) {

    if (str.empty()) {
        return false;
    }
    if (str.find(delimiter) == std::string::npos) {
        return false;
    }
    return true;
}

// Delete element from skip list
template<typename K, typename V>
void SkipList<K, V>::delete_element(K key) {

    mtx.lock();
    std::shared_ptr<Node<K, V>>current = this->_header;
    //std::shared_ptr<Node<K, V>>update[_max_level+1];
    std::vector<std::shared_ptr<Node<K, V>>> update(_max_level+1);

    //memset(update, 0, sizeof(Node<K, V>*)*(_max_level+1));

    // start from highest level of skip list
    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] !=nullptr && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current != nullptr && current->get_key() == key) {

        // start for lowest level and delete the current node of each level
        for (int i = 0; i <= _skip_list_level; i++) {

            // if at level i, next node is not target node, break the loop.
            if (update[i]->forward[i] != current)
                break;

            update[i]->forward[i] = current->forward[i];
        }

        // Remove levels which have no elements
        while (_skip_list_level > 0 && _header->forward[_skip_list_level] == 0) {
            _skip_list_level --;
        }

        _element_count --;
    }
    mtx.unlock();
    return;
}

// Search for element in skip list
/*
                           +------------+
                           |  select 60 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |
level 3         1+-------->10+------------------>50+           70       100
                                                   |
                                                   |
level 2         1          10         30         50|           70       100
                                                   |
                                                   |
level 1         1    4     10         30         50|           70       100
                                                   |
                                                   |
level 0         1    4   9 10         30   40    50+-->60      70       100
*/
template<typename K, typename V>
V SkipList<K, V>::search_element(K key) {

    std::shared_ptr<Node<K, V>>current = _header;

    // start from highest level of skip list
    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
    }

    //reached level 0 and advance pointer to right node, which we search
    current = current->forward[0];

    // if current node have key equal to searched key, we get it
    if (current and current->get_key() == key) {
        return current->get_value();
    }

    if(current) return current->get_value();
    if(!this->_header->forward[0])
    {
        std::cout << "no data!!"<<std::endl;
        exit(1);
    }
    return this->_header->forward[0]->get_value();
}


template<typename K, typename V>
bool SkipList<K, V>::find_element(K key,V *value)
{
    std::shared_ptr<Node<K, V>>current = _header;

    // start from highest level of skip list
    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
    }

    //reached level 0 and advance pointer to right node, which we search
    current = current->forward[0];

    // if current node have key equal to searched key, we get it
    if (current and current->get_key() == key) {
        *value = current->get_value();
        return 0;
    }

    if(current) 
    {
        *value = current->get_value();
        return 0;
    }
    return -1;
}

// construct skip list
template<typename K, typename V>
SkipList<K, V>::SkipList(int max_level) {

    this->_max_level = max_level;
    this->_skip_list_level = 0;
    this->_element_count = 0;

    // create header node and initialize key and value to nullptr
    K k;
    V v;
    this->_header = std::make_shared<Node<K, V>>(k, v, _max_level);
};

template<typename K, typename V>
SkipList<K, V>::~SkipList() {

    if (_file_writer.is_open()) {
        _file_writer.close();
    }
    if (_file_reader.is_open()) {
        _file_reader.close();
    }
}

template<typename K, typename V>
int SkipList<K, V>::get_random_level(){

    int k = 1;
    while (rand() % 2) {
        k++;
    }
    k = (k < _max_level) ? k : _max_level;
    return k;
};
// vim: et tw=100 ts=4 sw=4 cc=120
