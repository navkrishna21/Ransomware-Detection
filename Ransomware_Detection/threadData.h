#ifndef THREADDATA_H
#define THREADDATA_H

#include <windows.h>
#include <tchar.h>

typedef struct MyData {
	long long last_sucessful;
	LPCSTR path;
	bool* detected;
} MYDATA, *PMYDATA;

#endif