#ifndef __RESOURCE_READER_H__
#define __RESOURCE_READER_H__
#include <string>
#include "ByteBuffer.h"

using namespace std;

class Resource
{
public: 
    Resource(){}
	~Resource(){}
	inline string getType(){
		return type;
	}
	inline ByteBuffer getResouce(){
		return move(byteBuffer);
	}
private:
    friend class ResourceReader;
    string type;
	ByteBuffer byteBuffer;
};

class ResourceReader
{
public: 
static Resource getResource(string path);
};

#endif//__RESOURCE_READER_H__