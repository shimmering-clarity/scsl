#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__linux__)
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#if defined(DESKTOP_BUILD)
#include <iostream>
#include <ios>
#endif

#include "Arena.h"


#define ARENA_UNINIT	0
#define ARENA_STATIC	1
#define ARENA_ALLOC	2
#if defined(__linux__)
#define ARENA_MMAP	3
#define PROT_RW		PROT_READ|PROT_WRITE
#endif


void
InitializeArena(Arena &arena)
{
	arena.Store = NULL;
	arena.Size = 0;
	arena.Type = ARENA_UNINIT;
	arena.fd = 0;
}


int
NewStaticArena(Arena &arena, uint8_t *mem, size_t size)
{
	arena.Store = mem;
	arena.Size = size;
	arena.Type = ARENA_STATIC;
	return 0;
}


int
AllocNewArena(Arena & arena, size_t size)
{
	if (arena.Size > 0) {
		if (DestroyArena(arena) != 0) {
			return -1;
		}
	}

	arena.Type = ARENA_ALLOC;
	arena.Size = size;
	arena.Store = (uint8_t *)calloc(sizeof(uint8_t), size);
	if (arena.Store == NULL) {
		return -1;
	}

	return 0;
}


#if defined(__linux__)
int
MMapArena(Arena &arena, int fd, size_t size)
{
	if (arena.Size > 0) {
		if (DestroyArena(arena) != 0) {
			return -1;
		}
	}

	arena.Type = ARENA_MMAP;
	arena.Size = size;
	arena.Store = (uint8_t *)mmap(NULL, size, PROT_RW, MAP_SHARED, fd, 0);
	if ((void *)arena.Store == MAP_FAILED) {
		return -1;
	}
	arena.fd = fd;
	return 0;
}


int
OpenArena(Arena &arena, const char *path)
{
	struct stat	st;

	if (arena.Size > 0) {
		if (DestroyArena(arena) != 0) {
			return -1;
		}
	}

	if (stat(path, &st) != 0) {
		return -1;
	}

	arena.fd = open(path, O_RDWR); 
	if (arena.fd == -1) {
		return -1;
	}

	return MMapArena(arena, arena.fd, (size_t)st.st_size);
}


int
CreateArena(Arena &arena, const char *path, size_t size, mode_t mode)
{
	int	fd = 0;

	if (arena.Size > 0) {
		if (DestroyArena(arena) != 0) {
			return -1;
		}
	}

	fd = open(path, O_WRONLY|O_CREAT|O_TRUNC, mode);
	if (fd == -1) {
		return -1;
	}

	if (ftruncate(fd, size) == -1) {
		return -1;
	}

	close(fd);

	return OpenArena(arena, path);
}
#endif


/*
 * ClearArena clears the memory being used, removing any data
 * present. It does not free the memory; it is effectively a
 * wrapper around memset.
 */
void
ClearArena(Arena &arena)
{
	if (arena.Size == 0) {
		return;
	}

	memset(arena.Store, 0, arena.Size);
}


int
DestroyArena(Arena &arena)
{
	if (arena.Type == ARENA_UNINIT) {
		return 0;
	}

	switch (arena.Type) {
	case ARENA_STATIC:
		break;
	case ARENA_ALLOC:
		free(arena.Store);
		break;
	#if defined(__linux__)
	case ARENA_MMAP:
		if (munmap(arena.Store, arena.Size) == -1) {
			return -1;
		}

		if (close(arena.fd) == -1) {
			return -1;
		}

		arena.fd = 0;
		break;
	#endif
	default:
		#if defined(NDEBUG)
		return -1;
		#else
		abort();
		#endif

	}

	arena.Type = ARENA_UNINIT;
	arena.Size = 0;
	arena.Store = NULL;
	return 0;
}

#if defined(DESKTOP_BUILD)
void
DisplayArena(const Arena &arena)
{
	std::cout << "Arena @ 0x";
	std::cout << std::hex << (uintptr_t)&arena << std::endl;
	std::cout << std::dec;
	std::cout << "\tStore is " << arena.Size << " bytes at address 0x";
	std::cout << std::hex << (uintptr_t)&(arena.Store) << std::endl;
	std::cout << "\tType: ";

	switch (arena.Type) {
	case ARENA_UNINIT:
		std::cout << "uninitialized";
		break;
	case ARENA_STATIC:
		std::cout << "static";
		break;
	case ARENA_ALLOC:
		std::cout << "allocated";
		break;
#if defined(__linux__)
	case ARENA_MMAP:
		std::cout << "mmap/file";
		break;
#endif
	default:
		std::cout << "unknown (this is a bug)";
	}
	std::cout << std::endl;
}
#else
void
DisplayArena(const Arena &arena)
{

}
#endif

int
WriteArena(const Arena &arena, const char *path)
{
	FILE	*arenaFile = NULL;
	int	 retc = -1;

	arenaFile = fopen(path, "w");
	if (arenaFile == NULL) {
		return -1;
	}

	if (fwrite(arena.Store, sizeof(*arena.Store), arena.Size,
		   arenaFile) == arena.Size) {
		retc = 0;
	}

	if (fclose(arenaFile) != 0) {
		return -1;
	}

	return retc;
}

