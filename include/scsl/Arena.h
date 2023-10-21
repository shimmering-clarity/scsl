///
/// \file Arena.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-06
/// \brief Memory management using an arena.
///
/// Arena defines a memory management backend for pre-allocating memory.
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

#ifndef KIMODEM_ARENA_H
#define KIMODEM_ARENA_H


#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sys/stat.h>

#include "sctest/Exceptions.h"


#if defined(__WIN64__) || defined(__WIN32__) || defined(WIN32)

#include <Windows.h>
#include <fileapi.h>

#endif


namespace scsl {


/// \enum ArenaType
///
/// ArenaType describes the type of \class Arena.
enum class ArenaType
    : uint8_t {
	/// Uninit is an unintialized arena.
	Uninit,
	/// Static is an arena backed by a static block of memory.
	Static,
	/// Alloc is an arena backed by allocated memory.
	Alloc,
	/// MemoryMapped is an arena backed by a memory-mapped file.
	MemoryMapped,
};


/// Arena is the class that implements a memory arena.
///
/// The Arena uses the concept of a cursor to point to memory in the arena. The
/// #Start and #End methods return pointers to the start and end of the
/// arena memory.
///
/// The arena should be initialized with one of the Set methods (SetStatic,
/// SetAlloc) or one of the file-based options (Create, Open, MemoryMap). At
/// this point, no further memory management should be done until the end of the
/// arena's life, At which point Destroy should be called.
class Arena {
public:
	/// An Arena is initialized with no backing memory.
	Arena();

	~Arena();

	/// SetStatic points the arena to a chunk of memory. That memory is
	/// intended to be statically allocated, e.g. via a global `static
	/// uint8_t memory[memSize];`. If the arena is already backed, then
	/// #Destroy will be called first.
	///
	/// \param mem A pointer to a section of memory, preferably statically
	///    allocated.
	/// \param memSize The size of the memory section.
	/// \return Returns 0 on success and -1 on error.
	int SetStatic(uint8_t *mem, size_t memSize);

	/// SetAlloc allocates a chunk of memory for the arena; the arena takes
	/// ownership. If the arena is already backed, then #Destroy will be
	/// called first.
	///
	/// \param allocSize The size of memory to allocate.
	/// \return Returns 0 on success and -1 on error.
	int SetAlloc(size_t allocSize);


	/// MemoryMap points the arena to a memory-mapped file. This is
	/// currently only supported on Linux. If the arena is already backed,
	/// then #Destroy will be called first.
	///
	/// \param memFileDes File descriptor to map into memory.
	/// \param memSize The size of memory to map.
	/// \return Returns 0 on success and -1 on error.
#if defined(__posix__) || defined(__linux__) || defined(__APPLE__)
	int 	 MemoryMap(int memFileDes, size_t memSize);
#else

	int MemoryMap(int memFileDes, size_t memSize)
	{ (void)memFileDes; (void)memSize; throw NotImplemented("WIN32"); }

#endif
	/// Create creates a new file, truncating it if it already exists. On
	/// Unix-based platforms, the arena will be backed by a memory via
	/// #MemoryMap. On other platforms (e.g. Windows), the arena will read
	/// the file into an allocated arena, calling #SetAlloc.
	///
	/// \param path The path to the file that should be created.
	/// \param fileSize The size of the file to create.
	/// \return Returns 0 on success and -1 on error.
	int	 Create(const char *path, size_t fileSize);

	/// Open reads a file into the arena; the file must already exist. On
	/// Unix-based platforms, the arena will be backed by a memory via
	/// #MemoryMap. On other platforms (e.g. Windows), the arena will read
	/// the file into an allocated arena, calling #SetAlloc. On these
	/// platforms, in order to persist changes, #Write must be called to
	/// sync changes to disk.
	///
	/// \param path The path to the file to be loaded.
	/// \return Returns 0 on success and -1 on error.
	int 	 Open(const char *path);

	/// Start returns a pointer to the start of the memory in the arena.
	///
	/// \return A pointer to the start of the arena memory.
	uint8_t *Start() const
	{ return this->store; }

	/// End returns a pointer to the end of the arena memory.
	///
	/// \return A pointer to the end of the arena memory.
	uint8_t *End()
	{ return this->store + this->size; }

	/// CursorInArena checks whether the cursor is still in the arena.
	///
	/// \param cursor A pointer that ostensibly points to the arena's
	///    memory.
	/// \return True if the cursor is still in the arena.
	bool CursorInArena(const uint8_t *cursor);

	/// Returns the current size of the arena.
	///
	/// \return The size of the arena.
	size_t Size() const
	{ return this->size; }

	/// Type returns an ArenaType describing the arena.
	///
	/// \return An ArenaType describing the backing memory for the arena.
	ArenaType Type() const
	{ return this->arenaType; }

	/// Ready returns whether the arena is initialized.
	bool Ready() const
	{ return this->Type() != ArenaType::Uninit; };

	/// Clear zeroizes the memory in the arena.
	void Clear();

	/// Destroy removes any backing memory (e.g. from SetAlloc or
	/// MemoryMap). This does not call Clear; if the arena was backed by a
	/// file that should be persisted, it would wipe out the file.
	void Destroy();

	/// Write dumps the arena to a file suitable for loading by Open.
	///
	/// \warning DANGER: if arena is memory-mapped, DO NOT WRITE TO THE
	/// BACKING FILE!
	///
	/// \param path
	/// \return Returns 0 on success and -1 on error.
	int Write(const char *path);

	/// This operator allows the data in the arena to be accessed
	/// as if it were an array. If the index is out of bounds, it
	/// will throw a range_error.
	///
	/// \throws std::range_error.
	///
	/// \param index The index to retrieve.
	/// \return
	uint8_t &operator[](size_t index);

private:
	uint8_t *store;
	size_t size;
	int fd;
	ArenaType arenaType;
};


/// Write an Arena out to the output stream.
///
/// The resulting output looks something like
/// ```
/// Arena<allocated>@0x7fff91dfad70,store<128B>@0x0055d6c5881ec0.
///         ^          ^                    ^     ^
///         |          +- base memory       |     +- store memory
///         +- arena type                   +- arena size
/// ```
///
/// \param os
/// \param arena
/// \return
std::ostream &operator<<(std::ostream &os, Arena &arena);


} // namespace scsl


#endif
