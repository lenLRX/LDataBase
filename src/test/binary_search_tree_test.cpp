#include <test/test.h>
#include <log/log.h>
#include <ldb/tree/binary_search_tree/binary_search_tree.h>
#include <vector>
#include <random>
#include <algorithm>
#include <time.h>
using namespace std;
binary_search_tree<int,int> _tree;

const int test_scale = 100000;

vector<int> keys;
vector<int> values;

//www.cplusplus.com
struct c_unique {
  int current;
  c_unique() {current=0;}
  int operator()() {return ++current;}
} UniqueNumber;

mt19937_64 gen(time(nullptr));
uniform_int_distribution<int> distribution(1,10000);

int RandomNumber () { return (distribution(gen)); }

bool build_binary_search_tree(){
	keys.resize(test_scale);
	values.resize(test_scale);

	generate(keys.begin(),keys.end(),UniqueNumber);
	generate(values.begin(),values.end(),RandomNumber);

	shuffle(keys.begin(),keys.end(),gen);

	for(int i = 0;i < test_scale;i++){
		_tree.put(keys[i],values[i]);
	}

	return true;
}

bool check_binary_search_tree(){
	for(int i = 0;i < test_scale;i++){
		auto _got_value = _tree.get(keys[i]);
		if(values[i] != _got_value.second){
			cout << "key value not match expected: " << values[i] << " got " << _got_value.second << endl;
			return false;
		}
	}
	return true;
}

bool remove_values_binary_search_tree(){
	bool b = true;
	for(int i = 0;i < test_scale;i++){
		bool ret =  _tree.remove(keys[i]);
		b = b & ret;
	}
	return b;
}

ADDTEST(build_binary_search_tree)
ADDTEST(check_binary_search_tree)
ADDTEST(remove_values_binary_search_tree)