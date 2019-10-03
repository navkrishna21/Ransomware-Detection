#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "fileSystemWatcher.h"
#include "registryMonitor.h"
#include "vssadminDetector.h"
#include "threadData.h"
#include "myTimeLibrary.h"

//void main(int argc, char *argv[])
int _tmain(int argc, TCHAR *argv[])
{

	HANDLE  hThread_file_sytem_watcher, hThread_registry_monitor, hThread_vssadmin_detector;

	PMYDATA pData_vssadmin_detector, pData_registry_monitor, pData_file_sytem_watcher;

	pData_vssadmin_detector = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));
	pData_registry_monitor = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));
	pData_file_sytem_watcher = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));


	pData_vssadmin_detector->path = "vssadmin.exe"; //VSSAdmin process
	pData_file_sytem_watcher->path = "C:\\Users\\saint_boy\\Desktop\\new"; //Honeypot folder path to monitor
	pData_registry_monitor->path = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"; //Registry path to monitor
	
	//initialising last_successful values so that they are greater than maximum time to get infected
	pData_vssadmin_detector->last_sucessful = get_current_time() - MAX_TIME_TO_GET_INFECTED; 
	pData_file_sytem_watcher->last_sucessful = get_current_time() - 3*MAX_TIME_TO_GET_INFECTED;  
	pData_registry_monitor->last_sucessful = get_current_time() - 5*MAX_TIME_TO_GET_INFECTED; 
		
	bool ransomware_detected = false;
	
	pData_vssadmin_detector->detected = pData_file_sytem_watcher->detected = pData_registry_monitor->detected = &ransomware_detected;


	DWORD   dwThreadId1,dwThreadId2,dwThreadId3;

	hThread_vssadmin_detector = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		vssadmin_detector,       // thread function name
		pData_vssadmin_detector,          // argument to thread function 
		0,                      // use default creation flags 
		&dwThreadId1);   // returns the thread identifier

	hThread_file_sytem_watcher = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		Watcher,       // thread function name
		pData_file_sytem_watcher,          // argument to thread function 
		0,                      // use default creation flags 
		&dwThreadId2);   // returns the thread identifier

	hThread_registry_monitor = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		registry_monitor,       // thread function name
		pData_registry_monitor,          // argument to thread function 
		0,                      // use default creation flags 
		&dwThreadId3);   // returns the thread identifier
	
	printf("Ransomware detection has started\n");

	while (ransomware_detected == false) {

		if (abs(pData_vssadmin_detector->last_sucessful - pData_file_sytem_watcher->last_sucessful) <= MAX_TIME_TO_GET_INFECTED)
			ransomware_detected = true;

		if (abs(pData_vssadmin_detector->last_sucessful - pData_registry_monitor->last_sucessful) <= MAX_TIME_TO_GET_INFECTED)
			ransomware_detected = true;

		if (abs(pData_registry_monitor->last_sucessful - pData_file_sytem_watcher->last_sucessful) <= MAX_TIME_TO_GET_INFECTED)
			ransomware_detected = true;
	}

	printf("High possibility of a ransomware infection");

	CloseHandle(hThread_vssadmin_detector);
	if (pData_vssadmin_detector != NULL)
	{
		HeapFree(GetProcessHeap(), 0, pData_vssadmin_detector);
		pData_vssadmin_detector = NULL;    // Ensure address is not reused.
	}

	CloseHandle(hThread_registry_monitor);
	if (pData_registry_monitor != NULL)
	{
		HeapFree(GetProcessHeap(), 0, pData_registry_monitor );
		pData_registry_monitor = NULL;    // Ensure address is not reused.
	}

	CloseHandle(hThread_file_sytem_watcher);
	if (pData_file_sytem_watcher != NULL)
	{
		HeapFree(GetProcessHeap(), 0, pData_file_sytem_watcher );
		pData_file_sytem_watcher = NULL;    // Ensure address is not reused.
	}
	
	getchar();
	return 0;
}
