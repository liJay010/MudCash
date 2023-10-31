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

    void set_value(V);

    // Linear array to hold pointers to next node of different level
    std::vector<std::shared_ptr<Node<K, V>>> forward;
    //std::shared_ptr<Node<K, V>>*forward;

    int node_level;

private:
    K key;
    V value;
};

template<typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level) {
    this->key = k;
    this->value = v;
    this->node_level = level;

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

// Class template for Skip list
template <typename K, typename V>
class SkipList {

public:
    SkipList(int);
    ~SkipList();
    int get_random_level();
    std::shared_ptr<Node<K, V>> create_node(K, V, int);
    int insert_element(K, V);
    void update_element(K, V);
    void display_list();
    V search_element(K);
    bool find_element(K);
    std::vector<std::pair<std::string,std::string>> get_all_data();
    void delete_element(K);
    void clear();
    int size();
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
int SkipList<K, V>::insert_element(const K key, const V value) {

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

    std::shared_ptr<Node<K, V>>node = this->_header->forward[0];
    while (node != nullptr) {
        std::cout << node->get_key() << " : " << node->get_value() << endl;;
        node = node->forward[0];
    }
    std::cout << "*****Skip List*****"<<"\n";
}
template<typename K, typename V>
std::vector<std::pair<std::string,std::string>> SkipList<K, V>::get_all_data()
{
    std::vector<std::pair<std::string,std::string>> res;
    std::shared_ptr<Node<K, V>>node = this->_header->forward[0];
    while (node != nullptr) {
        res.push_back({node->get_key(),node->get_value()});
        node = node->forward[0];
    }
    return res;
}


// Get current SkipList size
template<typename K, typename V>
int SkipList<K, V>::size() {
    return _element_count;
}

template<typename K, typename V>
bool SkipList<K, V>::find_element(K key)
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
        return true;
    }

    return false;
}


template<typename K, typename V>
void SkipList<K, V>::update_element(K key,V value)
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
        current->set_value(value);
    }
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
        std::cout << "no data in this skiplist !!"<<std::endl;
        exit(1);
    }
    return this->_header->forward[0]->get_value();
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
void SkipList<K, V>::clear()
{
    this->_skip_list_level = 0;
    this->_element_count = 0;
    
    // create header node and initialize key and value to nullptr
    K k;
    V v;
    this->_header = std::make_shared<Node<K, V>>(k, v, _max_level);
}
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
