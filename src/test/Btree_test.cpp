#include <test/test.h>
#include <log/log.h>
#include <ldb/tree/Btree/Btree.h>
#include <vector>
#include <random>
#include <algorithm>
#include <time.h>
using namespace std;
using namespace BtreeNS;

static Btree<int,int> _tree(5);

static const int test_scale = 100000;

static vector<int> keys;
static vector<int> values;

//www.cplusplus.com
static struct c_unique {
  int current;
  c_unique() {current=0;}
  int operator()() {return ++current;}
} UniqueNumber;

static mt19937_64 gen(time(nullptr));
static uniform_int_distribution<int> distribution(1,10000);

static int RandomNumber () { return (distribution(gen)); }

bool build_Btree(){
	keys.resize(test_scale);
	values.resize(test_scale);

	generate(keys.begin(),keys.end(),UniqueNumber);
	generate(values.begin(),values.end(),RandomNumber);

	shuffle(keys.begin(),keys.end(),gen);

	for(int i = 0;i < test_scale;i++){
		_tree.put(keys[i],values[i]);
		//_tree.visualize();
	}

	return true;
}


bool check_Btree(){
	//_tree.visualize();
	for(int i = 0;i < test_scale;i++){
		auto _got_value = _tree.get(keys[i]);
		//cout << "i: " << i << endl;
		if(values[i] != _got_value.second){
			cout << "key " << keys[i]<< " value not match expected: " << values[i] << " got " << _got_value.second << endl;
			return false;
		}
	}
	return true;
}
/*
bool remove_values_binary_search_tree(){
	bool b = true;
	for(int i = 0;i < test_scale;i++){
		bool ret =  _tree.remove(keys[i]);
		b = b & ret;
	}
	return b;
}
*/

ADDTEST(build_Btree)
ADDTEST(check_Btree)
/*
ADDTEST(remove_values_binary_search_tree)
*/