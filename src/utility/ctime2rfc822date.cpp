#include "utility/ctime2rfc822date.h"

#include <map>

//ctime:Sat Sep 24 21:34:34 2016
//rfc822:Sun, 14 Aug 2005 16:13:03 UTC

enum class day{
	Mon,
	Tue,
	Wed,
	Thu,
    Fri,
	Sat,
	Sun
};

enum class month{
    Jan,
	Feb,
	Mar,
	Apr,
	May,
	Jun,
	Jul,
	Aug,
	Sep,
	Oct,
	Nov,
	Dec
};

string ctime2rfc822date(string ctime){
	int size = ctime.size();
	int i = 0;
	string d,m,y,date,time;
	for(;i < size;i++){
		if(ctime[i] == ' '){
			i++;
            break;
		}
		d.push_back(ctime[i]);
	}

	for(;i < size;i++){
		if(ctime[i] == ' '){
			i++;
            break;
		}
		m.push_back(ctime[i]);
	}

	for(;i < size;i++){
		if(ctime[i] == ' '){
			i++;
            break;
		}
		date.push_back(ctime[i]);
	}

	for(;i < size;i++){
		if(ctime[i] == ' '){
			i++;
            break;
		}
		time.push_back(ctime[i]);
	}

	for(;i < size;i++){
		if(ctime[i] == '\n'){
			i++;
            break;
		}
		y.push_back(ctime[i]);
	}


	string zone(tzname[0]);

	return d + ", " + date + " " + m + " " + y + " " + time + " " + zone;
}