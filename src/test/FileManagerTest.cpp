#include <test/test.h>
#include <log/log.h>
#include <ldb/file/FileManager.h>


bool FileManager_open(){
	string test_file_name = "/tmp/FileManager_open";
	FileManager test_file(test_file_name);

	if(access(test_file_name.c_str(),F_OK|R_OK) == 0)
	    return true;
	else
	    return false;
}

ADDTEST(FileManager_open)