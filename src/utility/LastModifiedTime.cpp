#include "utility/LastModifiedTime.h"

time_t LastModifiedTIme(string filename){
	struct stat _stat;
	stat(filename.c_str(),&_stat);
	return _stat.st_mtime;
}