#include "fileSystemWatcher.h"
#include "threadData.h"
#include "myTimeLibrary.h"

DWORD WINAPI Watcher(LPVOID lpParam) {

	PMYDATA pData = (PMYDATA)lpParam;
	LPCSTR path = pData->path;

	DWORD bytesReturned = 0;
	BOOL result = FALSE;

	HANDLE hDir = CreateFile(path,		//retrieve the handle to the directory
		FILE_LIST_DIRECTORY | STANDARD_RIGHTS_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);

	if (!hDir || hDir == INVALID_HANDLE_VALUE) //error in opening directory
	{
		wprintf(L"CreateFile failed\n");
		return 1;
	}

	while (*pData->detected==false) {

		char buf[256 * (sizeof(FILE_NOTIFY_INFORMATION))] = { 0 };	//declare and initialise the buffer to NULL for storing notifications

		result = ReadDirectoryChangesW(hDir,	//Get directory notifications
			buf,	//buffer
			sizeof(buf) / sizeof(*buf),	//size of buffer
			TRUE, /* monitor the entire subtree */
			FILE_NOTIFY_CHANGE_FILE_NAME |
			FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_ATTRIBUTES |
			FILE_NOTIFY_CHANGE_SIZE |
			FILE_NOTIFY_CHANGE_LAST_WRITE |
			FILE_NOTIFY_CHANGE_LAST_ACCESS |
			FILE_NOTIFY_CHANGE_CREATION |
			FILE_NOTIFY_CHANGE_SECURITY,
			&bytesReturned, //bytes transferred to buffer
			NULL,
			NULL);

		if (result && bytesReturned)	//check if there is no error
		{
			FILE_NOTIFY_INFORMATION* fni = (FILE_NOTIFY_INFORMATION*)buf;	//initialise to buffer for iterating notifications

			while (fni) {	//iterate until end of buffer is reached	

				switch (fni->Action) {		//check action
				case FILE_ACTION_ADDED:
					printf("A File is added\n");
					pData->last_sucessful = get_current_time();
					break;

				case FILE_ACTION_REMOVED:
					printf("A File is removed\n");
					pData->last_sucessful = get_current_time();
					break;

				case FILE_ACTION_MODIFIED:
					printf("A File is modified %ws\n", fni->FileName);
					pData->last_sucessful = get_current_time();
					break;

				case FILE_ACTION_RENAMED_OLD_NAME:
					printf("A File renamed, old name: %ws\n", fni->FileName);
					pData->last_sucessful = get_current_time();
					break;

				case FILE_ACTION_RENAMED_NEW_NAME:
					printf("A File renamed, new name: %ws\n", fni->FileName);
					pData->last_sucessful = get_current_time();
					break;

				default:
					printf("unknown action\n");
					pData->last_sucessful = get_current_time();
				}

				if (fni->NextEntryOffset)	//check offset between current and next notification
				{
					char *p = (char*)fni;
					fni = (FILE_NOTIFY_INFORMATION*)(p + fni->NextEntryOffset);	//jump the offset to get to next notification
				}
				else
				{
					fni = NULL;	//no Offset means end of the buffer
				}
			}
		}
		else {
			printf("ReadDirectoryChangesW failed\n");
		}

	}

	CloseHandle(hDir); //close the directory HANDLE
	
	return 0;
}