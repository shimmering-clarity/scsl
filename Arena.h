///
/// \file Arena.h
/// \author K. Isom
/// \date 2023-10-06
/// \brief Memory management using an arena.
///
/// \section COPYRIGHT
/// Copyright 2023 K. Isom <kyle@imap.cc>
///
/// Permission to use, copy, modify, and/or distribute this software for
/// any purpose with or without fee is hereby granted, provided that the
/// above copyright notice and this permission notice appear in all copies.
///
/// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
/// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
/// BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
/// OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
/// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
/// ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
/// SOFTWARE.
///
/// \section DESCRIPTION
/// Arena defines a memory management backend for pre-allocating memory.


#ifndef KIMODEM_ARENA_H
#define KIMODEM_ARENA_H


#include <iostream>
#include <sys/stat.h>
#include <cstddef>
#include <cstdint>


namespace klib {


/// \enum ArenaType describes the type of #Arena.
enum ArenaType : uint8_t {
	/// ARENA_UNINIT is an unintialized arena.
	ARENA_UNINIT,
	/// ARENA_STATIC is an arena backed by a static block of memory.
	ARENA_STATIC,
	/// ARENA_ALLOC is an arena backed by allocated memory.
	ARENA_ALLOC,
	/// ARENA_MMAP is an arena backed by a memory-mapped file.
	ARENA_MMAP,
};


/// Arena is the class that implements a memory arena.
class Arena {
public:
	Arena();

	~Arena();

	/*
	 * InitializeArena is intended for use only with systems that
	 * do not initialize new variables to zero. It should be called
	 * exactly once, at the start of the program. Any other time the
	 * arena needs to be reset, it should be called with Clear or
	 * Destroy.
	 */
	void Initialize();

	int SetStatic(uint8_t *, size_t);

	int SetAlloc(size_t allocSize);

#if defined(__linux__)
	int 	 MemoryMap(int memFileDes, size_t memSize); // Arena will own fd.
	int	 Create(const char *path, size_t fileSize, mode_t mode);
	int 	 Open(const char *path);
#elif defined(__WIN64__) || defined(__WIN32__)
	int	 Open(const char *path);
#endif

	uint8_t *NewCursor() const { return this->store; }
	uint8_t *End() { return this->store + this->size; }
	bool	 CursorInArena(const uint8_t *cursor);

	size_t Size() const
	{ return this->size; }

	uint8_t Type() const
	{ return this->arenaType; }

	uintptr_t	Store() { return (uintptr_t)this->store; }

	bool Ready() const { return this->Type() != ARENA_UNINIT; };
	void Clear();
	void Destroy(); /* dispose of any memory used by arena */

	/*
	 * DANGER: if arena is file backed (mmap or open), DO NOT WRITE TO THE
	 * BACKING FILE!
	 */
	int Write(const char *path);

	uint8_t &operator[](size_t index)
	{ return this->store[index]; }

private:
	uint8_t *store;
	size_t size;
	int fd;
	ArenaType arenaType;
};


std::ostream &operator<<(std::ostream& os, Arena &arena);


} // namespace klib


#endif
