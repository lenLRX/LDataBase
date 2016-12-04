#ifndef __BTREE_H__
#define __BTREE_H__
#include "../tree.h"
#include "Btree_node.h"
/*
https://en.wikipedia.org/wiki/B-tree

Definition
According to Knuth's definition, a B-tree of order m is a tree which satisfies the following properties:

Every node has at most m children.
Every non-leaf node (except root) has at least ⌈m/2⌉ children.
The root has at least two children if it is not a leaf node.
A non-leaf node with k children contains k−1 keys.
All leaves appear in the same level
*/


namespace BtreeNS
{
	template<typename K,typename V>
	class Btree:public tree<K,V>
	{
	public:
		Btree(int order):order(order),root(nullptr){}
		virtual ~Btree(){}

		virtual void put(const K& key,const V& value){
			if(root == nullptr){
				Leaf_node<K,V>* l = new Leaf_node<K,V>(order);
				l->put(key,value);
				root = l;
			}
			else{
				node<K,V>* leaf = root;
				while(true){
					node<K,V>* tmp = leaf->get_child_of(key);
					if(tmp == nullptr)
					    break;
					else
					    leaf = tmp;
				}

			    static_cast<Leaf_node<K,V>*>(leaf)->put(key,value);

                //back trace to split nodes
				node<K,V>* bt = leaf;
				
				while(bt != nullptr){
					if(bt->num_children){
						split(bt);
						bt = bt->parent;
					}
					else
					    break;
				}
			}
			
		}

		virtual bool remove(const K& key){
			;
		}

		virtual pair<bool,V> get(const K& key){
			;
		}
	private: 
        void split(node<K,V>* pnode){
			if(pnode->type == Leaf)
			    split_leaf(static_cast<Leaf_node<K,V>*>(pnode));
			else
			    split_Internal(static_cast<Internal_node<K,V>*>(pnode));
		}

		void split_Internal(Internal_node<K,V>* internal){
			Internal_node<K, V>* right_node = new Internal_node<K, V>(order);
			int half_order = 0.5 * order;
			int right_node_size = order - half_order;

			//  1  2  3  4  5
			//   \  \  \  \  \
			// 0  1  2  3  4  5
			//after split 
			//   1   2   |  4  5
			//    \   \  |   \  \
			// 0   1   2 | 3  4  5

			memmove(internal->keys + half_order + 1,
			right_node->keys,
			(right_node_size - 1) * sizeof(K));

			memmove(internal->children + half_order,
			right_node->children,
			right_node_size * sizeof(node<K,V>*));

			internal->num_children = half_order;
			right_node->num_children = right_node_size;

			if(internal->parent == nullptr){
				//this node was root 
				Internal_node<K,V>* next_root = new Internal_node<K,V>(order);
				next_root->num_children = 2;
				next_root->keys[0] = right_node->keys[0];
				next_root->children[0] = internal;
				next_root->children[1] = right_node;

				root = next_root;

				//set parents
				internal->parent = next_root;
				right_node->parent = next_root;
			}
			else{
				internal->parent->put(right_node);
				right_node->parent = internal->parent;
			}

		}

	    void split_leaf(Leaf_node<K,V>* leaf){
			// before split: this 
			// after split: this | right_node
			Leaf_node<K,V>* right_node = new Leaf_node<K,V>(order);

			int half_order = 0.5 * order;

			int right_node_size = order - half_order;

			memmove(leaf->keys + half_order,
			right_node->keys,
			right_node_size * sizeof(K));

			memmove(leaf->values + half_order,
			right_node->values,
			right_node_size * sizeof(V));

			leaf->num_children = half_order;
			right_node->num_children = right_node_size;

			if(leaf->parent == nullptr){
				//this leaf was root 
				Internal_node<K,V>* next_root = new Internal_node<K,V>(order);
				next_root->num_children = 2;
				next_root->keys[0] = right_node->keys[0];
				next_root->children[0] = leaf;
				next_root->children[1] = right_node;

				root = next_root;
				
				//set parents
				leaf->parent = next_root;
				right_node->parent = next_root;
			}
			else{
				//this leaf is not leaf
				leaf->parent->put(right_node);
				right_node->parent = leaf->parent;
			}

		}

	    int order;
		node<K,V>* root;
	};

};
#endif//__BTREE_H__