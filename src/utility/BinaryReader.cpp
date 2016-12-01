#include "utility/BinaryReader.h"
#include <unistd.h>
#include <fcntl.h>

ByteBuffer BinaryReader(string path){
	ByteBuffer ret;
	int fd = open(path.c_str(),O_RDONLY);
	if(fd < 0)
	    throw exception();
	char bunch[512] = {'\0'};
	while(true){
		bzero(bunch,512);
		int got = read(fd,bunch,512);
		if(got <=0)
		    break;
		ret.put(bunch,got);
	}
	return ret;
}