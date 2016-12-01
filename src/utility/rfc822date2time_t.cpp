#include "utility/rfc822date2time_t.h"
#include "log/log.h"
#include <sstream>
#include <map>



const map<string,int> month2int = {
    {"Jan",1},
	{"Feb",2},
	{"Mar",3},
	{"Apr",4},
	{"May",5},
	{"Jun",6},
	{"Jul",7},
	{"Aug",8},
	{"Sep",9},
	{"Oct",10},
	{"Nov",11},
	{"Dec",12}
};

time_t rfc822date2time_t(string rfc822date){
	struct tm btime;
	stringstream ss(rfc822date);
	string str;
	ss >> str;//ignore the day of weekdays 
	ss >> str;

	if(str[0] >= 48 && str[0] <= 57)
	{
		//rfc822
		btime.tm_mday = stoi(str);

		
		ss >> str;
		btime.tm_mon = month2int.at(str) - 1;

		
		ss >> str;
		btime.tm_year = stoi(str) - 1900;

	
		ss >> str;

		//hh:mm:ss

		string::iterator it = str.begin();
		string::iterator end = str.end();

		string h;

		for(;it != end;it++){
			if(*it == ':'){
				it++;
				break;
			}else{
				h.push_back(*it);
			}
		}

		btime.tm_hour = stoi(h);


		string m;

		for(;it != end;it++){
			if(*it == ':'){
				it++;
				break;
			}else{
				m.push_back(*it);
			}
		}

		btime.tm_min = stoi(m);


		string s;

		for(;it != end;it++){
			s.push_back(*it);
		}

		btime.tm_sec = stoi(s);
		btime.tm_isdst = 0;

		return mktime(&btime);
	}
	else{
		//chrome

		
		btime.tm_mon = month2int.at(str) - 1;

		ss >> str;

		//hh:mm:ss

		string::iterator it = str.begin();
		string::iterator end = str.end();

		string h;

		for(;it != end;it++){
			if(*it == ':'){
				it++;
				break;
			}else{
				h.push_back(*it);
			}
		}

		btime.tm_hour = stoi(h);


		string m;

		for(;it != end;it++){
			if(*it == ':'){
				it++;
				break;
			}else{
				m.push_back(*it);
			}
		}

		btime.tm_min = stoi(m);


		string s;

		for(;it != end;it++){
			s.push_back(*it);
		}

		btime.tm_sec = stoi(s);

		ss >> str;
		btime.tm_year = stoi(str) - 1900;

        ss >> str;
		btime.tm_mday = stoi(str);

		btime.tm_isdst = 0;

		return mktime(&btime);
	}
	
}