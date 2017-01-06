#ifndef __BINARY_SEARCH_TREE_NODE_H__
#define __BINARY_SEARCH_TREE_NODE_H__
#include <log/log.h>
template<typename K,typename V>
class binary_search_tree_node
{
public:
    binary_search_tree_node():parent(nullptr),left(nullptr),right(nullptr){}
    binary_search_tree_node(const K& key,const V& value):
    parent(nullptr),left(nullptr),right(nullptr),key(key),value(value){}

    void traversal(function<void(void*)>& fn){
        fn(this);
		if(left != nullptr)
		    left->traversal(fn);
		if(right != nullptr)
		    right->traversal(fn);
    }

    void remove_children_r(){
		if(left != nullptr)
		    left->remove_children_r();
		if(right != nullptr)
		    right->remove_children_r();
		delete left;
		delete right;
	}

    ~binary_search_tree_node() = default;

    binary_search_tree_node<K,V>*  parent;
    binary_search_tree_node<K,V>*  left;
    binary_search_tree_node<K,V>*  right;
    K key;
    V value;
};

#endif//__BINARY_SEARCH_TREE_NODE_H__