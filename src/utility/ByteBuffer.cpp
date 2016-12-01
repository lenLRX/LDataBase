#include "utility/ByteBuffer.h"
#include <cstring>

const int ByteBuffer::initial_capacity = 32;
const float ByteBuffer::incr_factor = 1.5f;

ByteBuffer::ByteBuffer():
buffer(nullptr),pointer(0),
buffersize(0),capacity(initial_capacity){
	buffer = (char*)malloc(capacity);
}

ByteBuffer::ByteBuffer(ByteBuffer&& other):
buffer(other.buffer),pointer(other.pointer),
buffersize(other.buffersize),capacity(other.capacity){
	other.pointer = 0;
	other.buffersize = 0;
	other.capacity = 0;
	other.buffer = nullptr;
}

ByteBuffer::ByteBuffer(string str):
buffer(nullptr),pointer(0),buffersize(0),
capacity(initial_capacity){
	put(str.data(),str.size());
}

ByteBuffer::ByteBuffer(const ByteBuffer& other)
:pointer(other.pointer),
buffersize(other.buffersize),capacity(other.capacity){
	if(buffer != nullptr){
		free(buffer);
	}
	buffer = (char*) malloc(capacity);
	memcpy(buffer,other.buffer,buffersize);
}

ByteBuffer::~ByteBuffer(){
	free(buffer);
}

ByteBuffer& ByteBuffer::operator = (ByteBuffer& other){
	if(this == &other)
	    return *this;
	//release current buffer
	free(buffer);
	buffer = other.buffer;
	other.buffer = nullptr;
	buffersize = other.buffersize;
	other.buffersize = 0;
	pointer = other.pointer;
	other.pointer = 0;
	capacity = other.capacity;
	other.capacity = 0;
	return *this;
}

ByteBuffer& ByteBuffer::operator = (string& str){
	put(str.data(),str.size());
	return *this;
}

ByteBuffer& ByteBuffer::operator = (string str){
	put(str.data(),str.size());
	return *this;
}

ByteBuffer& ByteBuffer::operator += (string& str){
	put(str.data(),str.size());
	return *this;
}

ByteBuffer& ByteBuffer::operator += (string str){
	put(str.data(),str.size());
	return *this;
}

ByteBuffer::operator string(){
	return string(buffer,buffersize);
}

ByteBuffer::operator bool(){
	return pointer < buffersize;
}

pair<void*,int> ByteBuffer::get(int nbytes){
	pair<void*,int> p;
	p.first = buffer + pointer;
	if(pointer + nbytes <= buffersize){
        p.second = nbytes;
		pointer += nbytes;
	}
	else{
        p.second = buffersize - pointer;
		pointer = buffersize;
	}
	    
	return p;
}

void ByteBuffer::put(const void* ptr,int nbytes){
	while(buffersize + nbytes > capacity){
		increase_capacity();
	}
	memcpy(buffer + buffersize,ptr,nbytes);
	buffersize += nbytes;
}

void ByteBuffer::put(void* ptr,int nbytes){
	while(buffersize + nbytes > capacity){
		increase_capacity();
	}
	memcpy(buffer + buffersize,ptr,nbytes);
	buffersize += nbytes;
}
