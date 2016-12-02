#ifndef __TREE_H__
#define __TREE_H__
#include <utility>

using std::pair;

template<typename K,typename V>
class tree
{
  public:
    tree() = default;
    virtual ~tree(){}
    virtual void put(const K& key,const V& value) = 0;
	virtual bool remove(const K& key) = 0;
    virtual pair<bool,V> get(const K& key) = 0;
};

#endif //__TREE_H__