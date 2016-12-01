#include <utility/TimeUtility.h>

using namespace TimeUtility;

time_t TimeUtility::getTime(){
	return time(NULL);
}

string TimeUtility::getCurrentRFC822Date(){
	time_t now = getTime();
	return ctime2rfc822date(ctime(&now));
}