#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
using std::string;
typedef off_t FIndex;
class FileManager
{
public: 
    FileManager(const string& FileName);
	FileManager() = delete;
	~FileManager();
	FIndex put(void* obj,size_t len);
	void* get(FIndex idx,size_t len);
private: 
    FIndex write_offset;
	int _fd;
};
#endif//__FILEMANAGER_H__