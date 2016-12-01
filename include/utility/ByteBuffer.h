#ifndef __BYTEBUFFER_H__
#define __BYTEBUFFER_H__
#include <utility>
#include <string>
#include <cstring>//memcpy
#include <cstdlib>
using namespace std;

class ByteBuffer
{
public:
    ByteBuffer();
	ByteBuffer(ByteBuffer&& other);
	ByteBuffer(string str);
	explicit ByteBuffer(const ByteBuffer& other);
	ByteBuffer& operator = (ByteBuffer& other);
	ByteBuffer& operator = (string& str);
	ByteBuffer& operator = (string str);
	ByteBuffer& operator += (string& str);
	ByteBuffer& operator += (string str);
	operator string();
	operator bool();
		
	~ByteBuffer();
	inline void rewind(){
		pointer = 0;
	}
	pair<void*,int> get(int nbytes);
	void put(const void* ptr,int nbytes);
	void put(void* ptr,int nbytes);
	inline int size(){
		return buffersize;
	}
private:
    inline void increase_capacity(){
		capacity *= incr_factor;
		//didn't check return value (which might be NULL)
		buffer = (char*)realloc(buffer,capacity);
	}

    char* buffer;
	int pointer;
	int buffersize;
	int capacity;
	const static int initial_capacity;
	const static float incr_factor;// capacity *= incr_factor;
};
#endif//__BYTEBUFFER_H__