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
#elif defined(__WIN64__) || defined(__WIN32__)
#include <Windows.h>
#include <fileapi.h>
#endif

#include <ios>

#include "Arena.h"


namespace klib {


Arena::Arena()
    : store(nullptr), size(0), fd(0), arenaType(ARENA_UNINIT)
{}


Arena::~Arena()
{
	this->Destroy();
}


void
Arena::Initialize()
{
	assert(this->arenaType != ARENA_UNINIT);
	this->store = nullptr;
	this->size = 0;
	this->arenaType = ARENA_UNINIT;
	this->fd = 0;
}


int
Arena::SetStatic(uint8_t *mem, size_t allocSize)
{
	this->store = mem;
	this->size = allocSize;
	this->arenaType = ARENA_STATIC;
	return 0;
}


int
Arena::SetAlloc(size_t allocSize)
{
	if (this->size > 0) {
		this->Destroy();
	}

	this->arenaType = ARENA_ALLOC;
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

	this->arenaType = ARENA_MMAP;
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
#elif defined(__WIN64__) || defined(__WIN32__)
int
Arena::Open(const char *path)
{
	HANDLE 		fHandle;
	size_t		fSize;
	size_t		fRead = 0;
	size_t		fRemaining;
	auto		cursor = this->store;
	OVERLAPPED	overlap;

	fHandle = CreateFileA(
	    (LPSTR)path,
	    GENERIC_READ,
	    (FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE),
	    NULL,
	    OPEN_ALWAYS,
	    FILE_ATTRIBUTE_NORMAL,
	    NULL);
	if (fHandle == INVALID_HANDLE_VALUE) {
		return -1;
	}

	if (!GetFileSizeEx(fHandle, reinterpret_cast<PLARGE_INTEGER>(&fSize))) {
		CloseHandle(fHandle);
		return -1;
	}

	this->SetAlloc(fSize);

	fRemaining = fSize;
	while (fRemaining != 0) {
		overlap.Offset = (fSize - fRemaining);
		if (!ReadFile(fHandle, cursor, fSize, reinterpret_cast<LPDWORD>(&fRead), &overlap)) {
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
	if (this->arenaType == ARENA_UNINIT) {
		return;
	}

	switch (this->arenaType) {
		case ARENA_STATIC:
			break;
		case ARENA_ALLOC:
			delete this->store;
			break;
#if defined(__linux__)
		case ARENA_MMAP:
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

	this->arenaType = ARENA_UNINIT;
	this->size = 0;
	this->store = nullptr;
	return;
}

std::ostream &
operator<<(std::ostream &os, Arena &arena)
{
	auto cursor = arena.Store();
	char cursorString[33] = {0};
	snprintf(cursorString, 32, "%#016llx", cursor);

	os << "Arena<";
	switch (arena.Type()) {
		case ARENA_UNINIT:
			os << "uninitialized";
			break;
		case ARENA_STATIC:
			os << "static";
			break;
		case ARENA_ALLOC:
			os << "allocated";
			break;
#if defined(__linux__)
		case ARENA_MMAP:
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


} // namespace klib