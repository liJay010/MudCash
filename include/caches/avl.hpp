#pragma once

#include<string>
#include<iostream>
using namespace std;

/*
定义了一个AVL树（平衡二叉搜索树）的数据结构，以及对该数据结构进行操作的类。
hash_Node 结构体：

key  - 是节点的键值，用于比较节点的大小。
ip_plus_port - 是与该键值相关联的字符串信息。
left 和 right - 是指向左子节点和右子节点的指针。
定义了默认构造函数，以及带有键值、字符串和子节点的构造函数。

avl_tree 类：
hash_Node* minValue(hash_Node* root);
hash_Node* maxValue(hash_Node* root);
void Suc(hash_Node* root, hash_Node*& pre, hash_Node*& suc, int key) 
hash_Node *insert(hash_Node *root, int key, string ip_plus_port)
hash_Node *deletehash_Node(hash_Node *root, int key)
inorder(hash_Node *root)


minValue() 和 maxValue() 方法用于找到树中最小和最大的节点。
Suc() 方法用于找到指定键值节点的前驱（predecessor）和后继（successor）节点。
insert() 方法用于将节点插入到树中。
deletehash_Node() 方法用于删除树中的指定键值节点。
inorder() 方法用于以中序遍历的方式打印树的节点信息。
*/
static int HASH_LEN = 32;
static const int RING_SIZE = 1e8;
static const uint32_t CRC32_POLY = 0xEDB88320;
static unsigned int CRC32(std::string s)
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
struct hash_Node {
    int key;
    string ip_plus_port;
    hash_Node* left;
    hash_Node* right; 
    hash_Node():key(0),ip_plus_port(""),left(NULL),right(NULL){}
    hash_Node(int key,string ip_plus_port,hash_Node* left,hash_Node* right)
    :key(key),ip_plus_port(ip_plus_port),left(left),right(right){}
}; 


// find min value of the tree
static hash_Node* minValue(hash_Node* node) { 
    hash_Node* current = node; 
    while (current->left != NULL) { 
        current = current->left; 
    } 
    return current; 
}    

// find max value of the tree
static hash_Node* maxValue(hash_Node* node) { 
    hash_Node* current = node; 
    while (current->right != NULL) { 
        current = current->right; 
    } 
    return current;
} 


// give succ and pre
static void Suc(hash_Node* root1, hash_Node*& pre, hash_Node*& suc, int key) {        
    if (root1 == NULL)  
        return ; 
    if (root1->key == key) { //base case
        if (root1->left != NULL) {    
            hash_Node* tmp = root1->left; 
            while (tmp->right) 
                tmp = tmp->right; 
            pre = tmp ; 
        }  
        if (root1->right != NULL) { 
            hash_Node* tmp = root1->right ; 
            while (tmp->left) 
                tmp = tmp->left ; 
            suc = tmp ; 
        } 
        return  ; 
    } 

    if (root1->key > key) { 
        suc = root1 ; 
        Suc(root1->left, pre, suc, key) ; 
    } 
    else { 
        pre = root1 ; 
        Suc(root1->right, pre, suc, key) ;   
    } 
} 


static void Suc_guard(hash_Node* root1, hash_Node*pre, hash_Node*suc, int key)
{
    Suc(root1, pre, pre, key);
    if(!pre) pre = maxValue(root1);
    if(!suc) suc = minValue(root1);
    if(!pre) cout << key <<  "  pre -> null" <<endl;
    if(!suc) cout << key <<  "suc -> null" <<endl;
}

static hash_Node *insert(hash_Node *node, int key, string ip_plus_port) {
    if (node == NULL) {//base case
        node = new hash_Node(key,ip_plus_port,NULL,NULL);
        return node; 
    }
    if (key < node->key) { 
        node->left  = insert(node->left, key,ip_plus_port); 
    }
    else {
        node->right = insert(node->right, key,ip_plus_port); 
    }
    return node; 
} 

static hash_Node *deletehash_Node(hash_Node *root1, int key) {
    if (root1 == NULL)
        return root1;
    if (key < root1->key)
        root1->left = deletehash_Node(root1->left, key);
    else if (key > root1->key)
        root1->right = deletehash_Node(root1->right, key);
    else {
        if (root1->left == NULL) {
            hash_Node *temp = root1->right;
            free(root1);
            return temp;
        }
        else if (root1->right == NULL) {
            hash_Node *temp = root1->left;
            free(root1);
            return temp;
        }
        hash_Node *temp = minValue(root1->right);
        root1->key = temp->key;
        root1->ip_plus_port=temp->ip_plus_port;
        root1->right = deletehash_Node(root1->right, temp->key);
    }
    return root1;
}

static void  inorder(hash_Node *root1) {
    if(root1==NULL) {
        return;
    }
    else {
        inorder(root1->left);
        cout << "hash code " <<root1->key << " " << root1->ip_plus_port << endl;
        inorder(root1->right);
    }
}
