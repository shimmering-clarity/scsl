//
// Created by kyle on 2023-10-10.
//

#ifndef KLIB_WINHELPERS_H
#define KLIB_WINHELPERS_H

#if defined(__WIN64__) || defined(__WIN32__) || defined(WIN32)

#include <Windows.h>
#include <winbase.h>
#include <fileapi.h>
#include <strsafe.h>

namespace klib {
namespace Windows {


int DisplayWinError(LPTSTR lpszFunction, HANDLE handle);

BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
);

HANDLE CreateFileWindows(const char *path);

int CreateFixedSizeFile(const char *path, size_t size);




} // namespace Windows
} // namespace klib

#endif // Windows-only guards.

#endif //KLIB_WINHELPERS_H