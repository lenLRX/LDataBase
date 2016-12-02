#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "log/log.h"
#include "test/test.h"

using namespace std;

int main(){
	srand(time(0));
	//Logger::getInstance().redirectStream("temp/log.txt");
	
	RUNALLTEST;

	SUMMARY;

	LOG << "TEST END" << endl;

	return 0;
}

