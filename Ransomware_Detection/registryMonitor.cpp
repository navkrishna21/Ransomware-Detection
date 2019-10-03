#include "registryMonitor.h"
#include "threadData.h"
#include "myTimeLibrary.h"

DWORD WINAPI registry_monitor(LPVOID lpParam) {

	PMYDATA pData = (PMYDATA)lpParam;
	LPCSTR path = pData->path;

	DWORD  dwFilter = REG_NOTIFY_CHANGE_NAME |
		REG_NOTIFY_CHANGE_ATTRIBUTES |
		REG_NOTIFY_CHANGE_LAST_SET |
		REG_NOTIFY_CHANGE_SECURITY;


	HANDLE hEvent;
	HKEY   hMainKey;
	HKEY   hKey;
	LONG   lErrorCode;

	//Registry path is specified here
	hMainKey = HKEY_CURRENT_USER;


	// Open a key.
	lErrorCode = RegOpenKeyEx(hMainKey, path, 0, KEY_NOTIFY, &hKey);
	if (lErrorCode != ERROR_SUCCESS)
	{
		_tprintf(TEXT("Error in RegOpenKeyEx (%d).\n"), lErrorCode);
		return 1;
	}


	// Create an event.
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hEvent == NULL)
	{
		_tprintf(TEXT("Error in CreateEvent (%d).\n"), GetLastError());
		return 1;
	}


	// Watch the registry key for a change of value.
	lErrorCode = RegNotifyChangeKeyValue(hKey,
		TRUE,
		dwFilter,
		hEvent,
		TRUE);

	if (lErrorCode != ERROR_SUCCESS)
	{
		_tprintf(TEXT("Error in RegNotifyChangeKeyValue (%d).\n"), lErrorCode);
		return 1;
	}


	// Wait for an event to occur.
	while (*pData->detected == false) {
		
		//_tprintf(TEXT("Waiting for a change in the specified key...\n"));
		
		if (WaitForSingleObject(hEvent, INFINITE) == WAIT_FAILED)
		{
			_tprintf(TEXT("Error in WaitForSingleObject (%d).\n"), GetLastError());
			return 1;
		}	
		else _tprintf(TEXT("Registry Change has occurred.\n"));
	
		pData->last_sucessful = get_current_time();

		Sleep(MAX_TIME_TO_GET_INFECTED);
	}
	


	// Close the key.
	lErrorCode = RegCloseKey(hKey);
	if (lErrorCode != ERROR_SUCCESS)
	{
		_tprintf(TEXT("Error in RegCloseKey (%d).\n"), GetLastError());
		return 1;
	}


	// Close the handle.
	if (!CloseHandle(hEvent))
	{
		_tprintf(TEXT("Error in CloseHandle.\n"));
		return 1;
	}

	return 0;

}