#ifndef __TEST_H__
#define __TEST_H__
#include <map>
#include <functional>
#include <string>
#include <vector>
#include <thread>
#include "log/log.h"
using namespace std;

extern vector<string> _test_details;
extern int test_count;
extern int success_count;

class TestManager
{
public:
	static TestManager& getInstance();
	map<string, function<bool(void)>> _test_functions;
	map<string, function<void(void)>> _test_bunch_functions;
private:

	TestManager() = default;
	~TestManager() = default;
};

#define __TEST_REGISTER_FUNC(testfunc)\
	int testfunc##TEST_REGISTER_FUNC(){\
	TestManager::getInstance()._test_functions[#testfunc] = function<bool(void)>(testfunc); \
	return 0;\
	}

#define ADDTEST(testfunc) \
	__TEST_REGISTER_FUNC(testfunc)\
	static int testfunc##TEST_REGISTER_FUNC_HELPER = testfunc##TEST_REGISTER_FUNC();

#define __TEST_REGISTER_BUNCH_FUNC(testbunchfunc)\
    int testbunchfunc##TEST_BUNCH_FUNC(){\
	TestManager::getInstance()._test_bunch_functions[#testbunchfunc] = \
	function<void(void)>(testbunchfunc);\
	return 0;\
	}

#define ADDBUNCHTEST(testbunchfunc)\
    __TEST_REGISTER_BUNCH_FUNC(testbunchfunc)\
	static int testbunchfunc##TEST_REGISTER_BUNCH_FUNC_HELPER = testbunchfunc##TEST_BUNCH_FUNC();

#define DECL_TEST_THREADS\
    vector<pair<string,thread>> localthreads;\
	map<string,bool> __FILE__##result;

#define ADD_A_TEST_TO_BUNCH(func)\
    localthreads.push_back(pair<string,thread>(\
	#func,\
	thread([&](){\
	__FILE__##result[#func] = func();})\
	));

#define WAIT_ALL_THREADS\
    for(auto& t:localthreads){\
	    t.second.join();\
		test_count++;\
		string detail = t.first;\
		if(__FILE__##result[detail]){\
		    detail +=  " success ";\
			success_count++;\
		}\
		else{\
		    detail += " fail ";\
		}\
		_test_details.push_back(detail);\
	}

#define RUNALLTEST \
do \
{\
	test_count = 0; \
	success_count = 0; \
for (pair<string, function<bool(void)>> p : TestManager::getInstance()._test_functions)\
{\
	test_count++; \
	string detail = p.first; \
if (p.second())\
{\
	detail += " success "; \
	success_count++; \
}\
	else\
{\
	detail += " fail "; \
}\
	_test_details.push_back(detail); \
}\
for (pair<string, function<void(void)>> p : TestManager::getInstance()._test_bunch_functions)\
{\
	string detail = p.first; \
	LOG << "running bunch test: " << detail << endl;\
	p.second();\
	_test_details.push_back(detail); \
}\
}while (0)

#define SUMMARY \
do \
{\
for (string line : _test_details)\
{\
	LOG << line << endl; \
}\
	LOG << success_count << " of " << test_count << " ( " \
	<< (float)success_count / test_count << " ) passed" << endl; \
} while (0)

#endif//__TEST_H__