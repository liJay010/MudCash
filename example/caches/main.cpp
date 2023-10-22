//
// Created by li on 2023-05-21.
//
//#include "SkipList.h"
#include "iostream"
#include "string"
#include "consistent_hash.h"
using namespace std;


int main() {
    consistent_hash hash;
    hash.add_real_node("192.168.2.136", 10);
    hash.add_real_node("192.168.2.137", 10);
    hash.add_real_node("192.168.2.138", 10);
    hash.add_real_node("192.168.2.139", 10);
    hash.print();
    for (int i = 0; i < 20; ++i) {
        string p = "stringnames";
        string res =  hash.search_by_name(p+ to_string(i));
        cout << p+ to_string(i) << "->"<< res << endl;
    }
    hash.drop_real_node("192.168.2.136");
    hash.print();
    for (int i = 0; i < 20; ++i) {
        string p = "stringnames";
        string res =  hash.search_by_name(p+ to_string(i));
        cout << p+ to_string(i) << "->"<< res << endl;
    }
    return 0;
}






