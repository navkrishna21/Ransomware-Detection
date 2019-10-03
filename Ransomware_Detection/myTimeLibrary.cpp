#include "myTimeLibrary.h"

long long get_current_time() {
	
	SYSTEMTIME currentSystemTime;
	GetSystemTime(&currentSystemTime);
	
	FILETIME v_ftime;
	ULARGE_INTEGER v_ui;
	SystemTimeToFileTime(&currentSystemTime, &v_ftime);
	v_ui.LowPart = v_ftime.dwLowDateTime;
	v_ui.HighPart = v_ftime.dwHighDateTime;

	return (long long)(v_ui.QuadPart/10000);
}