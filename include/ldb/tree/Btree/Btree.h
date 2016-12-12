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
		Btree(int order):order(order),half_order(order / 2),root(nullptr){}
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
			if(root == nullptr){
				return false;
			}else{
				Leaf_node<K,V>* leaf = getLeafNode(key);
				if(!leaf->remove(key)){
					return false;
				}

				//back trace to merge nodes
				node<K,V>* bt = leaf;

				while(bt != nullptr){
					if(bt->num_children <= half_order){
						//LOG << bt << endl;
						merge(bt);
						bt = bt->parent;
					}
					else
					    break;
				}
				return true;
			}
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

		void merge(node<K,V>* pnode){
			if(pnode->type == Leaf)
			    merge_Leaf(static_cast<Leaf_node<K,V>*>(pnode));
			else
			    merge_Internal(static_cast<Internal_node<K,V>*>(pnode));
		}

		void merge_Leaf(Leaf_node<K,V>* leaf){
			Internal_node<K,V>* pp = leaf->parent;
			if(pp == nullptr)
			    return;//it is root no need to merge
			else{
				if(pp->children[0] == leaf){
					//merge first child 
					Leaf_node<K,V>* rsibling = static_cast<Leaf_node<K,V>*>(pp->children[1]);
					if(rsibling->num_children > half_order){
						/*
						//borrowing one from right sibling
						//    7        12 
						//   / \        \
						//  /   \        \
						//  |   7 8 9     12 14 16 
						//  |
						//  4
						//
						//
						//    8        12 
						//   / \        \
						//  /   \        \
						//  |    8 9     12 14 16 
						//  |
						//  4 7
						*/   
						leaf->keys[leaf->num_children] = rsibling->keys[0];
						leaf->values[leaf->num_children] = rsibling->values[0];

						memmove(rsibling->keys,
						rsibling->keys + 1,
						(rsibling->num_children - 1) * sizeof(K));

						memmove(rsibling->values,
						rsibling->values + 1,
						(rsibling->num_children - 1) * sizeof(V));

						++(leaf->num_children);
						--(rsibling->num_children);

						//key of parent has to be changed too
						pp->keys[0] = rsibling->keys[0];
					}else{
						/*
						//merge the right sibling
						//    7        12 
						//   / \        \
						//  /   \        \
						//  |   7 8       12 14 16 
						//  |
						//  4
						//
						//
						//   12 
						//   / \ 
						//  /   \ 
						//  |    12 14 16 
						//  |
						//  4 7 8
						*/
						
						memmove(leaf->keys + leaf->num_children,
						rsibling->keys,
						rsibling->num_children * sizeof(K));

						memmove(leaf->values + leaf->num_children,
						rsibling->values,
						rsibling->num_children * sizeof(V));

						leaf->num_children += rsibling->num_children;

						//remove right sibling ref from parent
						memmove(pp->keys,
						pp->keys + 1,
						(pp->num_children - 2) * sizeof(K));

						memmove(pp->children + 1,
						pp->children + 2,
						(pp->num_children - 2) * sizeof(node<K,V>*));

						--(pp->num_children);
						delete rsibling;

						if(1 == pp->num_children && pp->parent == nullptr){
							root = leaf;
							leaf->parent = nullptr;
							//LOG << "delete leaf rsibling" << endl;
							delete pp;
						}
					}
				}else{
					for(int i = 0;i < pp->num_children - 1;i++){
						if(pp->children[i + 1] == leaf){
							Leaf_node<K,V>* lsibling = static_cast<Leaf_node<K,V>*>(pp->children[i]);
							if(lsibling->num_children > half_order){
								/*
								//borrowing one from left sibling
								//    10        12 
								//   / \        \
								//  /   \        \
								//  |    10        12 14 16 
								//  |
								//  4 7 9
								//
								//
								//    9        12 
								//   / \        \
								//  /   \        \
								//  |    9 10     12 14 16 
								//  |
								//  4 7
								*/

								memmove(leaf->keys + 1,
								leaf->keys,
								leaf->num_children * sizeof(K)
								);

								memmove(leaf->values + 1,
								leaf->values,
								leaf->num_children * sizeof(V)
								);

								leaf->keys[0] = lsibling->keys[lsibling->num_children - 1];
								leaf->values[0] = lsibling->values[lsibling->num_children - 1];

								++(leaf->num_children);
								--(lsibling->num_children);

								//key of parent has to be changed too
								pp->keys[i] = leaf->keys[0];
							}else{
								/*
								//merge the left sibling
								//    7        12 
								//   / \        \
								//  /   \        \
								//  |   7 8       12 14 16 
								//  |
								//  4
								//
								//
								//    4        12 
								//   / \        \
								//  /   \        \
								//  |    4 7 8    12 14 16 
								//  |
								//  ..
								*/

								memmove(leaf->keys + lsibling->num_children,
								leaf->keys,
								leaf->num_children * sizeof(K));

								memmove(leaf->values + lsibling->num_children,
								leaf->values,
								leaf->num_children * sizeof(V));

								memmove(leaf->keys,
								lsibling->keys,
								lsibling->num_children * sizeof(K));

								memmove(leaf->values,
								lsibling->values,
								lsibling->num_children * sizeof(V));

								leaf->num_children += lsibling->num_children;

								//remove left sibling ref from parent
								if(i != 0){
									memmove(pp->keys + i,
									pp->keys + i + 1,
									(pp->num_children - 2 - i) * sizeof(K));

									memmove(pp->children + i,
									pp->children + i + 1,
									(pp->num_children - 1 - i) * sizeof(node<K,V>*));
								}else{
									memmove(pp->keys,
									pp->keys + 1,
									(pp->num_children - 2) * sizeof(K));

									memmove(pp->children,
									pp->children + 1,
									(pp->num_children - 1) * sizeof(node<K,V>*));
								}
								

								--(pp->num_children);
								delete lsibling;

								if(1 == pp->num_children && pp->parent == nullptr){
									root = leaf;
									leaf->parent = nullptr;
									//LOG << "delete leaf lsibling" << endl;
									delete pp;
								}
							}
							break;
						}//if pp->children[i + 1] == leaf
				    }
				}
				
			}
		}

		void merge_Internal(Internal_node<K,V>* internal){
			Internal_node<K,V>* pp = internal->parent;
			//LOG << "pp was " << pp << endl;
			if(pp == nullptr)
			    return;//it is root no need to merge
			else{
				if(pp->children[0] == internal){
					//merge first child 
					Internal_node<K,V>* rsibling = static_cast<Internal_node<K,V>*>(pp->children[1]);
					if(rsibling->num_children > half_order){
						/*
						//borrowing one from right sibling (ignore graph below)
						//    6        12 
						//   / \        \
						//  /   \        \
						//  |   7 8 9     12 14 16 
						//  |   |
						//  4   6
						//  |\
						//  3 4 5 
						//
						//
						//    8        12 
						//   / \        \
						//  /   \        \
						//  |    8 9     12 14 16 
						//  |
						//  4 7
						*/   
						internal->keys[internal->num_children - 1] = rsibling->getNodeKey();
						internal->children[internal->num_children] = rsibling->children[0];
						internal->children[internal->num_children]->parent = internal;

						memmove(rsibling->keys,
						rsibling->keys + 1,
						(rsibling->num_children - 2) * sizeof(K));

						memmove(rsibling->children,
						rsibling->children + 1,
						(rsibling->num_children - 1) * sizeof(Internal_node<K,V>*));

						++(internal->num_children);
						--(rsibling->num_children);

						//key of parent has to be changed too
						pp->keys[0] = rsibling->getNodeKey();

						//LOG << "borrow rsibling" << endl;
					}else{
						/*
						//merge the right sibling
						//    7        12 
						//   / \        \
						//  /   \        \
						//  |   7 8       12 14 16 
						//  |
						//  4
						//
						//
						//   12 
						//   / \ 
						//  /   \ 
						//  |    12 14 16 
						//  |
						//  4 7 8
						*/

						K tmp = rsibling->getNodeKey();

						for(int j = 0;j < rsibling->num_children;j++){
							rsibling->children[j]->parent = internal;
						}

						internal->keys[internal->num_children - 1] = rsibling->getNodeKey();
						
						memmove(internal->keys + internal->num_children,
						rsibling->keys,
						(rsibling->num_children - 1) * sizeof(K));

						memmove(internal->children + internal->num_children,
						rsibling->children,
						rsibling->num_children * sizeof(node<K,V>*));

						internal->num_children += rsibling->num_children;

						//remove right sibling ref from parent
						memmove(pp->keys,
						pp->keys + 1,
						(pp->num_children - 2) * sizeof(K));

						memmove(pp->children + 1,
						pp->children + 2,
						(pp->num_children - 2) * sizeof(node<K,V>*));

						--(pp->num_children);
						delete rsibling;
						//std::cout << "merging rsibling : " << tmp << std::endl;
						if(1 == pp->num_children && pp->parent == nullptr){
							//it will be new root
							root = internal;
							internal->parent = nullptr;
							delete pp;
							//LOG << "Internal remove rsibling and  root pp : " << pp << endl;
						}

						//LOG << "merge rsibling" << endl;
					}
				}else{
					for(int i = 0;i < pp->num_children - 1;i++){
						if(pp->children[i + 1] == internal){
							Internal_node<K,V>* lsibling = static_cast<Internal_node<K,V>*>(pp->children[i]);
							if(lsibling->num_children > half_order){
								/*
								//borrowing one from left sibling
								//    10        12 
								//   / \        \
								//  /   \        \
								//  |    10        12 14 16 
								//  |
								//  4 7 9
								//
								//
								//    9        12 
								//   / \        \
								//  /   \        \
								//  |    9 10     12 14 16 
								//  |
								//  4 7
								*/

								K origin_key = internal->getNodeKey();

								memmove(internal->keys + 1,
								internal->keys,
								(internal->num_children - 1) * sizeof(K)
								);

								memmove(internal->children + 1,
								internal->children,
								internal->num_children * sizeof(node<K,V>*)
								);

								internal->children[0] = lsibling->children[lsibling->num_children - 1];
								internal->keys[0] = origin_key;
								
								internal->children[0]->parent = internal;

								++(internal->num_children);
								--(lsibling->num_children);

								//key of parent has to be changed too
								pp->keys[i] = internal->getNodeKey();
								//LOG << "borrow lsibling" << endl;
							}else{
								/*
								//merge the left sibling
								//    7        12 
								//   / \        \
								//  /   \        \
								//  |   7 8       12 14 16 
								//  |
								//  4
								//
								//
								//    4        12 
								//   / \        \
								//  /   \        \
								//  |    4 7 8    12 14 16 
								//  |
								//  ..
								*/

								K origin_key = internal->getNodeKey();
								//std::cout << "i: " << i << " num_children : " << internal->num_children << std::endl;
								K l_key = lsibling->getNodeKey();

								memmove(internal->keys + lsibling->num_children,
								internal->keys,
								(internal->num_children - 1) * sizeof(K));

								internal->keys[lsibling->num_children - 1] = origin_key;

								memmove(internal->children + lsibling->num_children,
								internal->children,
								internal->num_children * sizeof(node<K,V>*));

								memmove(internal->keys,
								lsibling->keys,
								(lsibling->num_children - 1) * sizeof(K));

								memmove(internal->children,
								lsibling->children,
								lsibling->num_children * sizeof(node<K,V>*));

								//update parent
								for(int j = 0;j < lsibling->num_children;j++){
									internal->children[j]->parent = internal;
								}

								internal->num_children += lsibling->num_children;

								//remove left sibling ref from parent
								if(i != 0){
									memmove(pp->keys + i,
									pp->keys + i + 1,
									(pp->num_children - 2 - i) * sizeof(K));

									memmove(pp->children + i,
									pp->children + i + 1,
									(pp->num_children - 1 - i) * sizeof(node<K,V>*));
								}else{
									memmove(pp->keys,
									pp->keys + 1,
									(pp->num_children - 2) * sizeof(K));

									memmove(pp->children,
									pp->children + 1,
									(pp->num_children - 1) * sizeof(node<K,V>*));
								}
								

								--(pp->num_children);
								delete lsibling;
								//std::cout << "num_children : " << internal->num_children << std::endl;
								//std::cout << "merging lsibling : " << l_key << " origin " << origin_key << std::endl;

								if(1 == pp->num_children && pp->parent == nullptr){
									root = internal;
									internal->parent = nullptr;
									delete pp;
									//LOG << "Internal remove lsibling and  root pp :" << pp << endl;
								}
								//LOG << "merge lsibling" << endl;
							}
							break;
						}//if pp->children[i + 1] == leaf
				    }
				}
				
			}
		}

        void split(node<K,V>* pnode){
			if(pnode->type == Leaf)
			    split_leaf(static_cast<Leaf_node<K,V>*>(pnode));
			else
			    split_Internal(static_cast<Internal_node<K,V>*>(pnode));
		}

		void split_Internal(Internal_node<K,V>* internal){
			int right_node_size = order - half_order;

			Internal_node<K, V>* right_node = new Internal_node<K, V>(order);

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
				Internal_node<K,V>* next_root = new Internal_node<K,V>(order);
				next_root->num_children = 2;
				next_root->keys[0] = right_node->getNodeKey();
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
				    parent_line << "|" << std::setw(6) << std::setfill(' ') << static_cast<Internal_node<K,V>*>(pleaf->parent)->getNodeKey() << "|";

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
				    parent_line << "|" << std::setw(6) << std::setfill(' ') << static_cast<Internal_node<K,V>*>(pinternal->parent)->getNodeKey() << "|";

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
		int half_order;
		node<K,V>* root;
	};

};
#endif//__BTREE_H__