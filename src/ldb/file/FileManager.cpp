#include <ldb/file/FileManager.h>
#include <log/log.h>

FileManager::FileManager(const string& FileName)
:tail_pos(0),_fd(-1)
{
	_fd = open(FileName.c_str(),O_RDWR|O_CREAT,
	S_IRUSR|S_IWUSR);
	struct stat st; 
	stat(FileName.c_str(), &st);
	tail_pos = st.st_size;
}

FIndex FileManager::alloc(size_t length){
	FIndex ret = tail_pos;
	tail_pos += length;
	return ret;
}

void FileManager::write(FIndex pos,void* obj,size_t length){
	ssize_t ret = pwrite(_fd,obj,length,pos);
	if(ret < 0 || static_cast<size_t>(ret) != length){
		LOG << ret << " bytes wrotten " << length << " bytes expected" << endl;
	}
}

void FileManager::get(FIndex idx,void* buffer,size_t length){
	ssize_t ret = pread(_fd, buffer,length, idx);
	if(ret < 0 || static_cast<size_t>(ret) != length){
		LOG << ret << " bytes read " << length << " bytes expected" << endl;
	}
}

FileManager::~FileManager()
{
	close(_fd);
}