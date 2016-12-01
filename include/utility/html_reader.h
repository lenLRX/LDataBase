#ifndef __HTML_READER_H__
#define __HTML_READER_H__
#include <string>
#include <fstream>
//#include "../network/http.h"
using namespace std;
extern const string html_type;
extern const string crlf;
string html_reader(string filename);
#endif//__HTML_READER_H__