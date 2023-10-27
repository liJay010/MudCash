#include "avl.hpp"
#include <iostream>
#include <string>
using namespace std;
/*
avl_tree 类：
hash_Node* minValue(hash_Node* root);
hash_Node* maxValue(hash_Node* root); 
void Suc(hash_Node* root, hash_Node*& pre, hash_Node*& suc, int key) 
hash_Node *insert(hash_Node *root, int key, string ip_plus_port) -> 返回root
hash_Node *deletehash_Node(hash_Node *root, int key) -> 返回root
inorder(hash_Node *root)
*/
int main()
{
    hash_Node *root=NULL;
    string s("112");
    root = insert(root, CRC32(s), s);
    root = insert(root, CRC32(s + "1"), s+ "1");
    root = insert(root, CRC32(s + "111"), s+ "111");
    root = insert(root, CRC32(s + "12"), s+ "12");
    hash_Node* p;
    hash_Node* n;
    Suc(root, p,n, CRC32("112")) ;
    inorder(root);
    cout << p->key << " " << n->key <<endl;
    
    return 0;
}