///
/// \file Arena.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-06
/// \brief Memory management using an arena.
///
/// Copyright 2023 K. Isom <kyle@imap.cc>
///
/// Permission to use, copy, modify, and/or distribute this software for
/// any purpose with or without fee is hereby granted, provided that
/// the above copyright notice and this permission notice appear in all /// copies.
///
/// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
/// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
/// AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
/// DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
/// OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
/// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
/// PERFORMANCE OF THIS SOFTWARE.
///

#include <cstdio>
#include <cstdlib>
#include <cstring>


#if defined(__posix__) || defined(__linux__) || defined(__APPLE__)
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define PROT_RW                (PROT_WRITE|PROT_READ)
#endif

#include <ios>

#include <scsl/Arena.h>


namespace scsl {


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


int
Arena::MemoryMap(int memFileDes, size_t memSize)
{
	if (this->size > 0) {
		this->Destroy();
	}

	this->arenaType = ArenaType::MemoryMapped;
	this->size = memSize;
	this->store = static_cast<uint8_t *>(mmap(nullptr, memSize, PROT_RW, MAP_SHARED,
				       memFileDes, 0));
	if (static_cast<void *>(this->store) == MAP_FAILED) {
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

	return this->MemoryMap(this->fd, static_cast<size_t>(st.st_size));
}


int
Arena::Create(const char *path, size_t fileSize)
{
	int 	ret = -1;

	if (this->size > 0) {
		this->Destroy();
	}


	auto *fHandle = fopen(path, "w");
	if (fHandle != nullptr) {
		auto newFileDes = fileno(fHandle);
		if (ftruncate(newFileDes, static_cast<off_t>(fileSize)) == 0) {
			ret = this->Open(path);
		}

		close(newFileDes);
		fclose(fHandle);
	}

	return ret;
}


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
			delete[] this->store;
			break;
#if defined(__posix__) || defined(__linux__) || defined(__APPLE__)
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
			return;
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
	auto cursor = arena.Start();
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
#if defined(__posix__) || defined(__linux__) || defined(__APPLE__)
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

#if defined(__posix__) || defined(__linux__) || defined(__APPLE__)
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
#if defined(SCSL_DESKTOP_BUILD) and !defined(SCSL_NOEXCEPT)
		throw std::range_error("index out of range");
#else
		abort();
#endif
	}
	return this->store[index];
}


} // namespace scsl
