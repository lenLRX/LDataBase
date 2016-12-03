#ifndef __BINARY_SEARCH_TREE_H__
#define __BINARY_SEARCH_TREE_H__
#include "../tree.h"
#include "binary_search_tree_node.h"

template<typename K,typename V>
class binary_search_tree:public tree<K,V>
{
	typedef binary_search_tree_node<K,V> tree_node;
public:
    binary_search_tree():tree<K,V>(),root(nullptr){}
	virtual ~binary_search_tree(){}

    virtual void put(const K& key,const V& value){
		if(root == nullptr){
			root = new tree_node(key,value);
		}
		else{
			tree_node* curr = root;

			while(true){
				if(key > curr->key){
					if(curr->right != nullptr){
						curr = curr->right;
					}else{
						//curr->right is null add node on it 
						curr->right = new tree_node(key,value);
						curr->right->parent = curr;
						return;
					}
				}
				else if(key < curr->key){
					if(curr->left != nullptr){
						curr = curr->left;
					}else{
						//curr->left is null add node on it 
						curr->left = new tree_node(key,value);
						curr->left->parent = curr;
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

	virtual bool remove(const K& key){
		tree_node* to_delete = get_ptr(key);
		if(to_delete == nullptr){
			return false;
		}else{
			if(to_delete->left == nullptr && to_delete->right == nullptr){
				//no child
				if(to_delete->parent == nullptr){
					root = nullptr;
				}
				else{
					if(to_delete->value > to_delete->parent->value){
						to_delete->parent->right = nullptr;
					}else{
						to_delete->parent->left = nullptr;
					}
				}
				
				delete to_delete;
				return true;
			}else if(to_delete->left != nullptr && to_delete->right != nullptr){
				//both child not null 
				//replace to_delete by biggest on the left 
				tree_node* right_most_lchild = to_delete->left;
				while(right_most_lchild->right != nullptr){
					right_most_lchild = right_most_lchild->right;
				}

				if(to_delete->parent == nullptr){
					//we are deleting root node
					//let left child be the new root
					//and connect right sub tree to right_most_lchild
					root = to_delete->left;
				}else{
					if(to_delete->key > to_delete->parent->key){
						to_delete->parent->right = to_delete->left;
					}else{
						to_delete->parent->left = to_delete->left;
					}
				}
				to_delete->left->parent = to_delete->parent;
				right_most_lchild->right = to_delete->right;
				to_delete->parent = right_most_lchild;
				delete to_delete;
				return true;
			}else{
				//one of sub tree is null 
				if(to_delete->left != nullptr){
					to_delete->left->parent = to_delete->parent;
					if(to_delete->parent == nullptr){
						root = to_delete->left;
					}else{
						if(to_delete->key > to_delete->parent->key){
							to_delete->parent->right = to_delete->left;
						}else{
							to_delete->parent->left = to_delete->left;
						}
					}
					to_delete->left->parent = to_delete->parent;
				}else{
					to_delete->right->parent = to_delete->parent;
					if(to_delete->parent == nullptr){
						root = to_delete->right;
					}else{
						if(to_delete->key > to_delete->parent->key){
							to_delete->parent->right = to_delete->right;
						}else{
							to_delete->parent->left = to_delete->right;
						}
					}
					to_delete->right->parent = to_delete->parent;
				}
				delete to_delete;
				return true;
			}
		}//to_delete not null
	}

    virtual pair<bool,V> get(const K& key){
		tree_node* curr = root;
		while(curr != nullptr){
			if(key > curr->key){
				curr = curr->right;
			}
			else if(key < curr->key){
				curr = curr->left;
			}else{
				// curr->key == key overwrite value 
				return pair<bool,V>(true,curr->value);
			}
		}
		return pair<bool,V>(false,V());
	}
private:
    tree_node* get_ptr(const K& key){
		tree_node* curr = root;
		while(curr != nullptr){
			if(key < curr->key){
				curr = curr->left;
			}
			else if(key > curr->key){
				curr = curr->right;
			}else{
				// return ptr
				return curr;
			}
		}
		return nullptr;
	}
    tree_node* root;
};
#endif//__BINARY_SEARCH_TREE_H__