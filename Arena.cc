#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(__linux__)

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define PROT_RW                (PROT_WRITE|PROT_READ)
#elif defined(__WIN64__) || defined(__WIN32__) || defined(WIN32)
#include <Windows.h>
#include <winbase.h>
#include <fileapi.h>
#include <strsafe.h>
#endif

#include <ios>

#include "Arena.h"


namespace klib {


Arena::Arena()
    : store(nullptr), size(0), fd(0), arenaType(ArenaType::Uninit)
{
}


Arena::~Arena()
{
	this->Destroy();
}


void
Arena::Initialize()
{
	assert(this->arenaType != ArenaType::Uninit);
	this->store = nullptr;
	this->size = 0;
	this->arenaType = ArenaType::Uninit;
	this->fd = 0;
}


int
Arena::SetStatic(uint8_t *mem, size_t memSize)
{
	this->store = mem;
	this->size = memSize;
	this->arenaType = ArenaType::Static;
	return 0;
}


int
Arena::SetAlloc(size_t allocSize)
{
	if (this->size > 0) {
		this->Destroy();
	}

	this->arenaType = ArenaType::Alloc;
	this->size = allocSize;
	this->store = new uint8_t[allocSize];
	if (this->store == nullptr) {
		return -1;
	}

	this->Clear();
	return 0;
}


#if defined(__linux__)

int
Arena::MemoryMap(int memFileDes, size_t memSize)
{
	if (this->size > 0) {
		this->Destroy();
	}

	this->arenaType = ArenaType::MemoryMapped;
	this->size = memSize;
	this->store = (uint8_t *) mmap(NULL, memSize, PROT_RW, MAP_SHARED,
				       memFileDes, 0);
	if ((void *) this->store == MAP_FAILED) {
		return -1;
	}
	this->fd = memFileDes;
	return 0;
}


int
Arena::Open(const char *path)
{
	struct stat st{};

	if (this->size > 0) {
		this->Destroy();
	}

	if (stat(path, &st) != 0) {
		return -1;
	}

	this->fd = open(path, O_RDWR);
	if (this->fd == -1) {
		return -1;
	}

	return this->MemoryMap(this->fd, (size_t) st.st_size);
}


int
Arena::Create(const char *path, size_t fileSize, mode_t mode)
{
	int newFileDes = 0;

	if (this->size > 0) {
		this->Destroy();
	}

	newFileDes = open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
	if (newFileDes == -1) {
		return -1;
	}

	if (ftruncate(newFileDes, fileSize) == -1) {
		return -1;
	}

	close(newFileDes);

	return this->Open(path);
}
#elif defined(__WIN64__) || defined(__WIN32__) || defined(WIN32)
static void
displayWinErr(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
	    FORMAT_MESSAGE_ALLOCATE_BUFFER |
	    FORMAT_MESSAGE_FROM_SYSTEM |
	    FORMAT_MESSAGE_IGNORE_INSERTS,
	    NULL,
	    dw,
	    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	    (LPTSTR) &lpMsgBuf,
	    0, NULL );

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
					  (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("%s failed with error %d: %s"),
			lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}


int
Arena::Open(const char *path)
{
	HANDLE 		 fHandle;
	DWORD 		 fRead = 0;
	size_t		 fSize;
	size_t		 fRemaining;
	auto		*cursor = this->store;
	OVERLAPPED	 overlap;

	std::cout << "CreateFileA\n";
	fHandle = CreateFileA(
	    (LPSTR)path,
	    GENERIC_READ,
	    (FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE),
	    NULL,
	    OPEN_ALWAYS,
	    FILE_ATTRIBUTE_NORMAL,
	    NULL);
	if (fHandle == INVALID_HANDLE_VALUE) {
		displayWinErr("CreateFileA");
		return -1;
	}

	std::cout << "GetFileSizeEx\n";
	if (!GetFileSizeEx(fHandle, reinterpret_cast<PLARGE_INTEGER>(&fSize))) {
		displayWinErr("GetFileSizeEx");
		CloseHandle(fHandle);
		return -1;
	}

	this->SetAlloc(fSize);

	fRemaining = fSize;
	while (fRemaining != 0) {
		std::cout << "ReadFile\n";
		overlap.Offset = (fSize - fRemaining);
		if (ReadFile(fHandle, cursor, fSize,
			     &fRead,
			     &overlap) != TRUE) {
			displayWinErr("ReadFile");
			CloseHandle(fHandle);
			this->Destroy();
			return -1;
		}

		cursor += fRead;
		fRemaining -= fRead;
	}

	CloseHandle(fHandle);
	return 0;
}


int
Arena::Create(const char *path, size_t fileSize, DWORD mode)
{
	HANDLE 		 fHandle;

	std::cout << "Create::CreateFileA\n";
	fHandle = CreateFileA(
	    (LPSTR)path,
	    GENERIC_READ|GENERIC_WRITE,
	    mode,
	    NULL,
	    CREATE_ALWAYS,
	    FILE_ATTRIBUTE_NORMAL,
	    NULL);
	if (fHandle == INVALID_HANDLE_VALUE) {
		displayWinErr("Create::CreateFileA");
		return -1;
	}

	std::cout << "SetFileValidData\n";
	if (SetFileValidData(fHandle, fileSize) != fileSize) {
		displayWinErr("SetFileValidData");
		CloseHandle(fHandle);
		return -1;
	}

	CloseHandle(fHandle);
	return this->Open(path);
}
#endif

bool
Arena::CursorInArena(const uint8_t *cursor)
{
	if (cursor < this->store) {
		return false;
	}

	if (cursor >= this->End()) {
		return false;
	}

	return true;
}


/*
 * ClearArena clears the memory being used, removing any data
 * present. It does not free the memory; it is effectively a
 * wrapper around memset.
 */
void
Arena::Clear()
{
	if (this->size == 0) {
		return;
	}

	memset(this->store, 0, this->size);
}


void
Arena::Destroy()
{
	if (this->arenaType == ArenaType::Uninit) {
		return;
	}

	switch (this->arenaType) {
		case ArenaType::Static:
			break;
		case ArenaType::Alloc:
			delete this->store;
			break;
#if defined(__linux__)
		case ArenaType::MemoryMapped:
			if (munmap(this->store, this->size) == -1) {
				abort();
				return;
			}

			if (close(this->fd) == -1) {
				abort();
			}

			this->fd = 0;
			break;
#endif
		default:
#if defined(NDEBUG)
			return -1;
#else
			abort();
#endif

	}

	this->arenaType = ArenaType::Uninit;
	this->size = 0;
	this->store = nullptr;
	return;
}

std::ostream &
operator<<(std::ostream &os, Arena &arena)
{
	auto cursor = arena.NewCursor();
	char cursorString[33] = {0};
	snprintf(cursorString, 32, "%#016llx",
			(long long unsigned int)cursor);

	os << "Arena<";
	switch (arena.Type()) {
		case ArenaType::Uninit:
			os << "uninitialized";
			break;
		case ArenaType::Static:
			os << "static";
			break;
		case ArenaType::Alloc:
			os << "allocated";
			break;
#if defined(__linux__)
		case ArenaType::MemoryMapped:
			os << "mmap/file";
			break;
#endif
		default:
			os << "unknown (this is a bug)";
	}
	os << ">@0x";
	os << std::hex << (uintptr_t) &arena;
	os << std::dec;
	os << ",store<" << arena.Size() << "B>@";
	os << std::hex << cursorString;
	os << std::dec;

	return os;
}


int
Arena::Write(const char *path)
{
	FILE *arenaFile = NULL;
	int retc = -1;

#if defined(__posix__) || defined(__linux__)
	arenaFile = fopen(path, "w");
	if (arenaFile == nullptr) {
#else
		if (fopen_s(&arenaFile, path, "w") != 0) {
#endif
		return -1;
	}

	if (fwrite(this->store, sizeof(*this->store), this->size,
		   arenaFile) == this->size) {
		retc = 0;
	}

	if (fclose(arenaFile) != 0) {
		return -1;
	}

	return retc;
}

uint8_t &
Arena::operator[](size_t index)
{
		if (index > this->size) {
#if defined(DESKTOP_BUILD) and !defined(KLIB_NO_ASSERT)
			throw std::range_error("index out of range");
#else
			abort();
#endif
		}
		return this->store[index];
}


} // namespace klib
