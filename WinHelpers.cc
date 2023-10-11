//
// Created by kyle on 2023-10-10.
//

#if defined(__WIN64__) || defined(__WIN32__) || defined(WIN32)

#include "WinHelpers.h"


namespace klib {
namespace Windows {


int
DisplayWinError(LPTSTR lpszFunction, HANDLE handle)
{
	// Retrieve the system error message for the last-error code
	DWORD dw = GetLastError();
#ifndef NDEBUG
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;

	FormatMessage(
	    FORMAT_MESSAGE_ALLOCATE_BUFFER |
	    FORMAT_MESSAGE_FROM_SYSTEM |
	    FORMAT_MESSAGE_IGNORE_INSERTS,
	    NULL,
	    dw,
	    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	    (LPTSTR) &lpMsgBuf,
	    0, NULL);

	// Display the error message and exit the process
	lpDisplayBuf = (LPVOID) LocalAlloc(LMEM_ZEROINIT,
					   (lstrlen((LPCTSTR) lpMsgBuf) +
					    lstrlen((LPCTSTR) lpszFunction) +
					    40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR) lpDisplayBuf,
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("%s failed with error %d: %s"),
			lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR) lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
#endif
	if ((handle != NULL) && (handle != INVALID_HANDLE_VALUE)) {
		CloseHandle(handle);
	}
	return dw;
}


BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
	    NULL,            // lookup privilege on local system
	    lpszPrivilege,   // privilege to lookup
	    &luid))        // receives LUID of privilege
	{
		printf("LookupPrivilegeValue error: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if (!AdjustTokenPrivileges(
	    hToken,
	    FALSE,
	    &tp,
	    sizeof(TOKEN_PRIVILEGES),
	    (PTOKEN_PRIVILEGES) NULL,
	    (PDWORD) NULL)) {
		printf("AdjustTokenPrivileges error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
		printf("The token does not have the specified privilege. \n");
		return FALSE;
	}

	return TRUE;
}


HANDLE
CreateFileWindows(const char *path)
{
	HANDLE fHandle;

	return CreateFileA(
	    (LPSTR) path,
	    (GENERIC_READ | GENERIC_WRITE),
	    (FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE),
	    NULL,
	    CREATE_ALWAYS,
	    FILE_ATTRIBUTE_NORMAL,
	    NULL);
}


int
CreateFixedSizeFile(const char *path, size_t size)
{
	_LARGE_INTEGER fileSize;

	fileSize.QuadPart = size;

	HANDLE fHandle = CreateFileWindows(path);
	if (SetFilePointerEx(fHandle, fileSize, nullptr, FILE_BEGIN) != TRUE) {
		return DisplayWinError("SetFilePointerEx", fHandle);
	}

	if (SetEndOfFile(fHandle) != TRUE) {
		return DisplayWinError("SetEndOfFile", fHandle);
	}

	CloseHandle(fHandle);
	return 0;
}


} // namespace Windows
} // namespace klib


#endif