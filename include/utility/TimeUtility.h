#ifndef __TIMEUTILITY_H__
#define __TIMEUTILITY_H__
#include "LastModifiedTime.h"
#include "ctime2rfc822date.h"
#include "rfc822date2time_t.h"

namespace TimeUtility{
	string getCurrentRFC822Date();
	time_t getTime();
}

#endif//__TIMEUTILITY_H__