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
#include "WinHelpers.h"
#pragma comment(lib, "advapi32.lib")
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
Arena::Create(const char *path, size_t fileSize)
{
	FILE *fHandle = nullptr;
	int newFileDes = 0;

	if (this->size > 0) {
		this->Destroy();
	}


	fHandle = fopen(path, "w");
	if (fHandle == nullptr) {
		return -1;
	}
	newFileDes = fileno(fHandle);

	if (ftruncate(newFileDes, fileSize) == -1) {
		return -1;
	}

	close(newFileDes);
	return this->Open(path);
}
#elif defined(__WIN64__) || defined(__WIN32__) || defined(WIN32)
int
Arena::Open(const char *path)
{
	HANDLE fHandle;
	DWORD fRead = 0;
	size_t fSize;
	size_t fRemaining;
	auto *cursor = this->store;
	OVERLAPPED overlap = {0};

	fHandle = CreateFileA(
	    (LPSTR) path,
	    GENERIC_READ,
	    (FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE),
	    NULL,
	    OPEN_ALWAYS,
	    FILE_ATTRIBUTE_NORMAL,
	    NULL);
	if (fHandle == INVALID_HANDLE_VALUE) {
		return Windows::DisplayWinError("CreateFileA", NULL);
	}

	if (SetFilePointer(fHandle, 0, 0, FILE_BEGIN) != 0) {
		return Windows::DisplayWinError("SetFilePointer", fHandle);
	}

	if (GetFileSizeEx(fHandle, reinterpret_cast<PLARGE_INTEGER>(&fSize)) !=
	    TRUE) {
		return Windows::DisplayWinError("GetFileSizeEx", fHandle);
	}

	this->SetAlloc(fSize);
	cursor = this->NewCursor();

	this->store[0] = 1;
	fRemaining = fSize;
	while (fRemaining != 0) {
		overlap.Offset = (fSize - fRemaining);
		if (ReadFile(fHandle, cursor, fSize - 1,
			     &fRead,
			     &overlap) != TRUE) {
			auto errorCode = GetLastError();
			if (errorCode != ERROR_HANDLE_EOF) {
				this->Destroy();

				return Windows::DisplayWinError("ReadFile", fHandle);
			}
			break;
		}

		cursor += fRead;
		fRemaining -= fRead;
	}

	CloseHandle(fHandle);
	return 0;
}


int
Arena::Create(const char *path, size_t fileSize)
{
	auto	errorCode = Windows::CreateFixedSizeFile(path, fileSize);
	if (errorCode != 0) {
		return errorCode;
	}
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
		 (long long unsigned int) cursor);

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
	FILE *arenaFile = nullptr;
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
