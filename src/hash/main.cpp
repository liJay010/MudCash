#include "avl.hpp"
#include <iostream>
#include <string>
#include "SkipList.h"
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
    SkipList<string,string> skip(18);
    skip.insert_element("a","b");
    skip.insert_element("b","ff");
    skip.insert_element("c","s");
    skip.insert_element("d","s");
    skip.update_element("d","54454154156");

    skip.display_list();
    cout << "size " << skip.size();
    skip.delete_element("d");
    skip.clear();
    cout << "size " << skip.size();
    skip.display_list();

    std::vector<std::pair<std::string,std::string>> res = skip.get_all_data();
    for(auto x : res)
    {
        cout << x.first << " " << x.second <<endl;
    }
    
    return 0;
}