#ifndef __BINARY_SEARCH_TREE_H__
#define __BINARY_SEARCH_TREE_H__
#include "../tree.h"
#include "binary_search_tree_node.h"

template<typename K,typename V>
class binary_search_tree:public tree<K,V>
{
	typedef binary_search_tree_node<K,V> tree_node;
public:
    binary_search_tree():root(nullptr){}

    virtual void put(K key,V value){
		if(root == nullptr){
			root = new tree_node(key,value);
		}
		else{
			tree_node* curr = root;

			while(true){
				if(curr->key > key){
					if(curr->right != nullptr){
						curr = curr->right;
					}else{
						//curr->right is null add node on it 
						curr->right = new tree_node(key,value);
						return;
					}
				}
				else if(curr->key < key){
					if(curr->left != nullptr){
						curr = curr->left;
					}else{
						//curr->left is null add node on it 
						curr->left = new tree_node(key,value);
						return;
					}
				}else{
					// curr->key == key overwrite value 
					curr->value = value;
					return;
				}
			}//while(true)
		}
	}

	virtual void remove(K key);

    virtual pair<bool,V> get(K key){
		tree_node* curr = root;
		while(curr != nullptr){
			if(curr->key > key){
				curr = curr->right;
			}
			else if(curr->key < key){
				curr = curr->left;
			}else{
				// curr->key == key overwrite value 
				return pair<bool,V>(true,curr->value);
			}
		}
		return pair<bool,V>(false,V());
	}
private:
    tree_node* root;
};
#endif//__BINARY_SEARCH_TREE_H__