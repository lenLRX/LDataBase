#ifndef __BINARY_SEARCH_TREE_NODE_H__
#define __BINARY_SEARCH_TREE_NODE_H__

template<typename K,typename V>
class binary_search_tree_node
{
public:
   binary_search_tree_node():left(nullptr),right(nullptr),parent(nullptr){}
   binary_search_tree_node(K& key,V& value):key(key),value(value),
   left(nullptr),right(nullptr),parent(nullptr){}

   ~binary_search_tree_node() = default;

   binary_search_tree_node<K,V>*  parent;
   binary_search_tree_node<K,V>*  left;
   binary_search_tree_node<K,V>*  right;
   K key;
   V value;
};

#endif//__BINARY_SEARCH_TREE_NODE_H__