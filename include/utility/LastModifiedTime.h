#ifndef __LASTMODIFIEDTIME_H__
#define __LASTMODIFIEDTIME_H__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
using namespace std;

time_t LastModifiedTIme(string filename);

#endif//__LASTMODIFIEDTIME_H__