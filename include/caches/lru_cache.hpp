#ifndef lru_cache
#define lru_cache

#include<iostream>
#include<string>
#include<unordered_map>
using namespace std;
/*

lrucache 类：

unordered_map<string, ListNode*> memo 用于将键值映射到对应的链表节点。
head 和 tail 是伪头节点和伪尾节点，它们用于简化边界条件的处理。
capacity 表示缓存的容量。
pushToHead 方法将节点插入到链表头部。
moveToHead 方法将已存在的节点移动到链表头部（表示最近使用）。
delTail 方法删除链表尾部的节点（表示最近最少使用）。
put_in_cache 方法用于将键值对存入缓存。如果键已存在，更新其值并将节点移到头部；如果不存在，创建新节点并将其插入到头部，如果缓存已满，则删除尾部节点。
get_from_cache 方法用于根据键获取缓存中的值。如果存在，将节点移到头部并返回值；如果不存在，返回空字符串。
delete_from_cache 方法用于从缓存中删除指定键值对。如果存在，删除节点并从哈希表中移除。

这个LRU缓存的实现使用了双向链表和哈希表，保证了在O(1)的时间复杂度内完成插入、查询和删除操作。
LRU缓存是一种常用的缓存策略，保留了最近使用的数据，当缓存满时，替换最近最少使用的数据，以保持缓存中的数据是最新和最有用的。
*/

class ListNode {
public:
	string key;
	string value;
	ListNode *prev;
	ListNode *next;
	ListNode():key(""),value(""),prev(NULL),next(NULL){}
	ListNode(string key, string value):key(key),value(value),prev(NULL),next(NULL) {}
};

class lrucache {
private:
    unordered_map<string,ListNode*> memo;
    ListNode* head=new ListNode;
    ListNode* tail=new ListNode;
    int capacity;

 	void pushToHead(ListNode* node)
	{
    	node->next=head->next;
		head->next->prev=node;
		node->prev=head;
		head->next=node;
 	}
 	void moveToHead(ListNode* node)
 	{
    	node->prev->next=node->next;
    	node->next->prev=node->prev;
    	pushToHead(node);
 	}
	void delTail(ListNode* node)
 	{
    	node->prev->next=tail;
    	tail->prev=node->prev;
 	}

public:
	lrucache(int capacity):capacity(capacity) {
        head->next=tail;
        tail->prev=head;
    }
	~lrucache() {
		for(auto& i:memo) {
			delete i.second;
		}
	}

	void display()
	{
		ListNode *cur=head->next;
		while(cur->next!=NULL)
		{
			cout<<"key: "<<cur->key<<" value: "<<cur->value<<endl;
			cur=cur->next;
		}
	}

 	void put_in_cache(string key,string value)
 	{
    	if(memo.count(key))
    	{
        	memo[key]->value=value;
        	moveToHead(memo[key]);
    	}
   	 	else
    	{
        	ListNode* node=new ListNode(key,value);
        	pushToHead(node);
        	memo[key]=node;
        	if(memo.size()>capacity)
        	{
            	ListNode* temp=tail->prev;
            	delTail(temp);
            	memo.erase(temp->key);
				delete temp;
        	}
    	}
 	} 

  	string get_from_cache(string key)
  	{
    	if(memo.count(key))
    	{
        	moveToHead(memo[key]);
        	return memo[key]->value;
    	}
    	else
    	{
        	return "";
    	}
  	}

	void delete_from_cache(string key)
	{	
  		if(memo.count(key))
  		{
	      	ListNode* del=memo[key];
	        del->prev->next=del->next;
			del->next->prev=del->prev;
			memo.erase(key);
			delete del;
      	} 
	}
};



#endif