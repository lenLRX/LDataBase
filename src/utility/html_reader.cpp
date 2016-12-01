#include "utility/html_reader.h"

const string html_type = "text/html;charset=utf-8";
const string crlf = "\r\n";

string html_reader(string filename){
	string ret;
	ifstream html(filename);
	char buffer[512];
	
	while(html.getline(buffer,512)){
		ret += string(buffer) + crlf;
	}
	html.close();
	return ret;
}