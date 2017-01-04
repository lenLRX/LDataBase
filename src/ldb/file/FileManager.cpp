#include <ldb/file/FileManager.h>

FileManager::FileManager(const string& FileName)
:write_offset(0),_fd(-1)
{
	_fd = open(FileName.c_str(),O_RDWR|O_CREAT,
	S_IRUSR|S_IWUSR);
	struct stat st; 
	stat(FileName.c_str(), &st);
	write_offset = st.st_size;
}

FileManager::~FileManager()
{
	close(_fd);
}