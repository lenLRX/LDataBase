#ifndef __BTREE_H__
#define __BTREE_H__
#include "../tree.h"
#include "Btree_node.h"
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
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

		void visualize(){
			std::map<int,std::string> datas;
			show_node(root,datas,0);
			for(int i = 0;i < datas.size();i++){
				std::cout << datas[i] << std::endl;
			}
		}

		virtual void put(const K& key,const V& value){
			if(root == nullptr){
				Leaf_node<K,V>* l = new Leaf_node<K,V>(order);
				l->put(key,value);
				root = l;
			}
			else{
				Leaf_node<K,V>* leaf = getLeafNode(key);
				leaf->put(key,value);

                //back trace to split nodes
				node<K,V>* bt = leaf;
				
				while(bt != nullptr){
					if(bt->num_children == order){
						split(bt);
						bt = bt->parent;
					}
					else
					    break;
				}
			}
			
		}

		virtual bool remove(const K& key){
			return false;
		}

		virtual pair<bool,V> get(const K& key){
			Leaf_node<K,V>* leaf = getLeafNode(key);
			if(nullptr == leaf)
			    return std::pair<bool,V>(false,V());
			else
			    return leaf->get(key);
		}
	private: 
	    Leaf_node<K,V>* getLeafNode(const K& key){
			node<K,V>* leaf = root;
			while(true){
				node<K,V>* tmp = leaf->get_child_of(key);
				if(tmp == nullptr)
				    break;
				else
				    leaf = tmp;
			}
			return static_cast<Leaf_node<K,V>*>(leaf);
		}

        void split(node<K,V>* pnode){
			if(pnode->type == Leaf)
			    split_leaf(static_cast<Leaf_node<K,V>*>(pnode));
			else
			    split_Internal(static_cast<Internal_node<K,V>*>(pnode));
		}

		void split_Internal(Internal_node<K,V>* internal){
			
			int half_order = 0.5 * order;
			int right_node_size = order - half_order;

			Internal_node<K, V>* right_node = new Internal_node<K, V>(internal->keys[half_order], order);

			/*
			// even:
			//  1  2  3  4  5
			//   \  \  \  \  \
			// 0  1  2  3  4  5
			// after split 
			//   1   2   |  4  5
			//    \   \  |   \  \
			// 0   1   2 | 3  4  5
			//
			// odd:
			//  1 2 3 4
			//   \ \ \ \
			//  0 1 2 3 4
			// after split  half_order: 2  right_node_size: 3
			//  1   | 3 4
			//   \  |  \ \
			//  0 1 | 2 3 4
			*/

			memmove(right_node->keys,
			internal->keys + half_order,
			(right_node_size - 1) * sizeof(K));

			memmove(right_node->children,
			internal->children + half_order,
			right_node_size * sizeof(node<K,V>*));

			internal->num_children = half_order;
			right_node->num_children = right_node_size;

			if(internal->parent == nullptr){
				//this node was root 
				Internal_node<K,V>* next_root = new Internal_node<K,V>(
					right_node->node_key,order);
				next_root->num_children = 2;
				next_root->keys[0] = right_node->node_key;
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

			for(int i = 0;i < right_node->num_children;i++){
				right_node->children[i]->parent = right_node;
			}

		}

	    void split_leaf(Leaf_node<K,V>* leaf){
			// before split: this 
			// after split: this | right_node
			Leaf_node<K,V>* right_node = new Leaf_node<K,V>(order);

			int half_order = 0.5 * order;

			int right_node_size = order - half_order;

			memmove(right_node->keys,
			leaf->keys + half_order,
			right_node_size * sizeof(K));

			memmove(right_node->values,
			leaf->values + half_order,
			right_node_size * sizeof(V));

			leaf->num_children = half_order;
			right_node->num_children = right_node_size;

			if(leaf->parent == nullptr){
				//this leaf was root 
				Internal_node<K,V>* next_root = new Internal_node<K,V>(right_node->keys[0],order);
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

		void show_node(node<K,V>* p,std::map<int,std::string>& datas,int depth){
			if(p->type == Leaf){
				Leaf_node<K, V>* pleaf = static_cast<Leaf_node<K, V>*>(p);
				std::string sepline;
				sepline += "|";
				for(int i = 0;i < pleaf->num_children * 7 - 1;i++){
					sepline += "-";
				}
				sepline += "|";

				std::stringstream parent_line;

                if(pleaf->parent == nullptr)
				    parent_line << "|  NULL|";
				else
				    parent_line << "|" << std::setw(6) << std::setfill(' ') << pleaf->parent->node_key << "|";

				for(int i = 1;i < pleaf->num_children;i++){
					parent_line << "      |";
				}

				std::stringstream keys_line;
				keys_line << "|";
				for(int i = 0;i < pleaf->num_children;i++){
					keys_line << std::setw(6) << std::setfill(' ') << pleaf->keys[i] << "|";
				}

				std::stringstream vals_line;
				vals_line << "|";
				for(int i = 0;i < pleaf->num_children;i++){
					vals_line << std::setw(6) << std::setfill(' ') << pleaf->values[i] << "|";
				}

				/*
				sepline
				parent_line
				sepline
				keys
				sepline 
				values 
				seplines
				emptyline

				total:8
				*/
				datas[8*depth + 0] += sepline + " ";
				datas[8*depth + 1] += parent_line.str() + " ";
				datas[8*depth + 2] += sepline + " ";
				datas[8*depth + 3] += keys_line.str() + " ";
				datas[8*depth + 4] += sepline + " ";
				datas[8*depth + 5] += vals_line.str() + " ";
				datas[8*depth + 6] += sepline + " ";
				datas[8*depth + 7] += " ";
			}
			else
			{
				//internal 
				Internal_node<K, V>* pinternal = static_cast<Internal_node<K, V>*>(p);
				std::string sepline;
				sepline += "|";
				for(int i = 0;i < pinternal->num_children * 7 - 1;i++){
					sepline += "-";
				}
				sepline += "|";

				std::stringstream parent_line;

                if(pinternal->parent == nullptr)
				    parent_line << "|  NULL|";
				else
				    parent_line << "|" << std::setw(6) << std::setfill(' ') << pinternal->parent->node_key << "|";

				for(int i = 1;i < pinternal->num_children;i++){
					parent_line << "      |";
				}

				std::stringstream keys_line;
				keys_line << "|";
				for(int i = 0;i < pinternal->num_children - 1;i++){
					keys_line << std::setw(6) << std::setfill(' ') << pinternal->keys[i] << "|";
				}
				keys_line << "      |";//Internal_node has n - 1 keys

				std::stringstream vals_line;
				vals_line << "|";
				for(int i = 0;i < pinternal->num_children;i++){
					vals_line << std::setw(6) << std::setfill(' ') << pinternal->children[i]->keys[0] << "|";
				}

				/*
				sepline
				parent_line
				sepline
				keys
				sepline 
				values 
				seplines
				emptyline

				total:8
				*/
				datas[8*depth + 0] += sepline + " ";
				datas[8*depth + 1] += parent_line.str() + " ";
				datas[8*depth + 2] += sepline + " ";
				datas[8*depth + 3] += keys_line.str() + " ";
				datas[8*depth + 4] += sepline + " ";
				datas[8*depth + 5] += vals_line.str() + " ";
				datas[8*depth + 6] += sepline + " ";
				datas[8*depth + 7] += " ";

				for(int i = 0;i < pinternal->num_children;i++){
					show_node(pinternal->children[i],datas,depth + 1);
				}
			}
		}

	    int order;
		node<K,V>* root;
	};

};
#endif//__BTREE_H__