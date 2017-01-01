#ifndef __BPlusTree_NODE_H__
#define __BPlusTree_NODE_H__

#include <utility>//for std::pair
#include <cstdlib>
#include <cstring>//for memmove
#include <log/log.h>

namespace BPlusTreeNS
{
	enum node_type{
		Internal = 0,
		Leaf = 1,
	};

    //forward decl
	template<typename K,typename V>
	class Internal_node;

	template<typename K,typename V>
	class node
	{
	public:
		node(int order,node_type type):
		order(order),num_children(0),type(type),
		parent(nullptr),keys(nullptr){}

		virtual node<K,V>* get_child_of(const K& key) = 0;
		virtual ~node(){}
		int order;
		int num_children;
		node_type type;
		Internal_node<K,V>* parent;
		K* keys;
	};

	template<typename K,typename V>
	class Leaf_node:public node<K,V>
	{
	public:
	    using node<K,V>::order;
        using node<K,V>::num_children;
		using node<K,V>::parent;
		using node<K,V>::keys;
	    Leaf_node(int order):node<K,V>(order,Leaf){
			keys = new K[order];
			values = new V[order];
		}

		virtual ~Leaf_node(){
			delete[] keys;
			delete[] values;
		}

		void put(const K& key,const V& value){
			if(num_children == 0){
				keys[0] = key;
				values[0] = value;
			}else{
				bool done = false;

				int insert_before = 0;

				for(;insert_before < num_children;insert_before++){
					if(keys[insert_before] > key){
						//move K,Vs 1 pos back 
						memmove(keys + insert_before + 1,
						keys + insert_before,
						(num_children - insert_before) * sizeof(K));

						memmove(values + insert_before + 1,
						values + insert_before,
						(num_children - insert_before) * sizeof(V));

						keys[insert_before] = key;
						values[insert_before] = value;

						done = true;
						break;
					}else if(keys[insert_before] == key){
						keys[insert_before] = key;
						values[insert_before] = value;

						done = true;
						break;
					}
				}

				if(!done){
					// insert to the back
					keys[num_children] = key;
					values[num_children] = value;
				}
			}
			
			++num_children;
		}

		bool remove(K key){
			for(int i = 0;i < num_children;i++){
				//remove 5   2 4 5 7 9
				//
				//           2 4 7 9
				if(key == keys[i]){
					memmove(keys + i,
					keys + i + 1,
					(num_children - i - 1) * sizeof(K)
					);

					memmove(values + i,
					values + i + 1,
					(num_children - i - 1) * sizeof(V)
					);

					--num_children;
					if(i == 0 && parent != nullptr){
						//first element changed
						parent->updateKey(this,keys[0]);
					}
					return true;
				}
			}
			return false;
		}

		std::pair<bool,V> get(const K& key){
			for(int i = 0 ;i < num_children;++i){
				if(keys[i] == key){
					return std::pair<bool,V>(true,values[i]);
				}
			}
			return std::pair<bool,V>(false,V());
		}

		virtual node<K,V>* get_child_of(const K& key){
			return nullptr;
		}
		
		V* values;
	};

	template<typename K,typename V>
	class Internal_node:public node<K,V>
	{
	public:
	    using node<K,V>::order;
        using node<K,V>::num_children;
		using node<K,V>::parent;
		using node<K,V>::keys;

	    Internal_node(int order):node<K,V>(order,Internal){
			keys = new K[order - 1];
			children = new node<K,V>*[order];
		}
		virtual ~Internal_node(){
			delete[] keys;
			delete[] children;
		}

		void updateKey(node<K,V>* child,const K& key){
			//LOG << "update Key" << endl;
			if(child == children[0]){
				if(parent != nullptr)
				    parent->updateKey(this,getNodeKey());
			}else{
				for(int i = 1;i < num_children;i++){
					if(child == children[i]){
						keys[i - 1] = key;
					}
				}
			}
		}

		K& getNodeKey(){
			if(children[0]->type == Leaf){
				return children[0]->keys[0];
			}else{
				return static_cast<Internal_node<K,V>*>(children[0])->getNodeKey();
			}
		}

		virtual node<K,V>* get_child_of(const K& key){
			/*
			//  get 11
			//  5  9  12
			//   \  \   \
			//  0 5  9   12
			//  num_children : 4
			//  return 2
			*/

			for(int i = 0;i < num_children - 1;i++){
				if(key < keys[i]){
					return children[i];
				}
			}
			return children[num_children - 1];
		}

		void put(node<K,V>* child){
			if(child->type == Leaf){
				put(static_cast<Leaf_node<K,V>*>(child));
			}
			else{
				put(static_cast<Internal_node<K,V>*>(child));
			}
		}

		void put(Leaf_node<K,V>* child){
			const K& child_key = child->keys[0];

			child->parent = this;

			if(child_key < children[0]->keys[0]){
				/*
				//new child small than all children thus push it to the fisrt slot.
				//before insert 3:
				//    5
				//     \
				//   4  7
				//after insert 3:
				//    4   5
				//     \   \
				//   3  4   5
				*/

				memmove(keys + 1,
				keys,
				(num_children - 1) * sizeof(K));

				memmove(children + 1,
				children,
				num_children * sizeof(node<K,V>*));

				keys[0] = children[1]->keys[0];
				children[0] = child;
			}else{
				/*
				//before insert 7:
				//     3   6   9
				//      \   \   \
				//    2  3   6   9
				//after insert 7 (7 < key[2]):
				//     3  6  7  9
				//      \  \  \  \
				//     2 3  6  7  9
				*/
				bool done = false;

				for(int i = 0;i < num_children - 1;i++){
					if(child_key < keys[i]){
						memmove(keys + i + 1,
						keys + i,
						(num_children - 1 - i) * sizeof(K));

						memmove(children + i + 2,
						children + i + 1,
						(num_children - 1 - i) * sizeof(node<K,V>*));

						keys[i] = child_key;
						children[i + 1] = child;

						done = true;
						break;
					}
				}

				if(!done){
					//put new child to the back
					keys[num_children - 1] = child_key;
					children[num_children] = child;
				}
			}

			++num_children;

		}

		void put(Internal_node<K,V>* child){
			const K& child_key = child->getNodeKey();

			child->parent = this;

			if(child_key < children[0]->keys[0]){
				/*
				//new child small than all children thus push it to the fisrt slot.
				//before insert 3:
				//    5
				//     \
				//   4  7
				//after insert 3:
				//    4   5
				//     \   \
				//   3  4   5
				*/

				memmove(keys + 1,
				keys,
				(num_children - 1) * sizeof(K));

				memmove(children + 1,
				children,
				num_children * sizeof(node<K,V>*));

				keys[0] = static_cast<Internal_node<K,V>*>(children[1])->getNodeKey();
				children[0] = child;
			}else{
				/*
				//before insert 7:
				//     3   6   9
				//      \   \   \
				//    2  3   6   9
				//after insert 7 (7 < key[2]):
				//     3  6  7  9
				//      \  \  \  \
				//     2 3  6  7  9
				*/
				bool done = false;

				for(int i = 0;i < num_children - 1;i++){
					if(child_key < keys[i]){
						memmove(keys + i + 1,
						keys + i,
						(num_children - 1 - i) * sizeof(K));

						memmove(children + i + 2,
						children + i + 1,
						(num_children - 1 - i) * sizeof(node<K,V>*));

						keys[i] = child_key;
						children[i + 1] = child;

						done = true;
						break;
					}
				}

				if(!done){
					//put new child to the back
					keys[num_children - 1] = child_key;
					children[num_children] = child;
				}
			}

			++num_children;

		}

		bool remove(K key){
			bool ret = false;
			if(key == getNodeKey()){
				/*
				// remove smallest key(1)
				// num_children : 5
				//   3  5  9  13
				//    \  \  \  \
				//   1 3  5  9  13
				//
				//  after remove
				//   5  9  13
				//    \  \  \
				//   3 5  9  13
				*/

				memmove(keys,
				keys + 1,
				(num_children - 2) * sizeof(K)
				);

				memmove(children,
				children + 1,
				(num_children - 1) * sizeof(V)
				);
				ret = true;
			}else{
				/*
				// remove 9
				// num_children : 5
				// 3  5  9  13
				//  \  \  \  \
				// 1 3  5  9  13
				//
				// after remove
				// 3  5  13
				//  \  \  \
				// 1 3  5  13
				*/
				for(int i = 0;i < num_children - 1;i++){
					if(keys[i] == key){
						memmove(keys + i,
						keys + i + 1,
						(num_children - 2 - i) * sizeof(K)
						);

						memmove(children + i + 1,
						children + i + 2,
						(num_children - 2 - i) * sizeof(V)
						);
						ret = true;
						break;
					}
				}
			}

			if(ret)
			    --num_children;
			return ret;
		}

		node<K,V>** children;
	};
};



#endif//__BPlusTree_NODE_H__