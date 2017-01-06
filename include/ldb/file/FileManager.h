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
	FIndex alloc(size_t length);
	void write(FIndex pos,void* obj,size_t length);
	void get(FIndex idx,void* buffer,size_t length);
private: 
    FIndex tail_pos;
	int _fd;
};
#endif//__FILEMANAGER_H__