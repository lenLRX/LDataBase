#ifndef __LOG_H__
#define __LOG_H__
#include <fstream>
#include <iostream>
#include <string>
#include <errno.h>
#include "utility/TimeUtility.h"

using namespace std;
#define LOG Logger::getInstance() << TimeUtility::getCurrentRFC822Date() + string(" ") << __FILE__ << " line " << __LINE__ << ":"
class Logger
{
  public:
    static Logger &getInstance();
    void redirectStream(string filename);
    void flush();
    template <typename T>
    Logger &operator<<(const T &obj)
    {
	stream << obj;
	return *this;
    }

    Logger &operator<<(ostream &(*_Pfn)(ostream &));

    ~Logger();

  private:
    fstream _fstream;
    ostream stream;
    Logger();
};

#endif
