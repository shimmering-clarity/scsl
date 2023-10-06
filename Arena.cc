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

#include "Arena.h"


#define ARENA_UNINIT	0
#define ARENA_STATIC	1
#define ARENA_ALLOC	2
#define ARENA_MMAP	3

#define PROT_RW		PROT_READ|PROT_WRITE


int
new_arena(Arena &arena, uint8_t *mem, size_t size)
{
	arena.store = mem;
	arena.size = size;
	arena.type = ARENA_STATIC;
	return 0;
}


int
alloc_new_arena(Arena &arena, size_t size)
{
	if (arena.size > 0) {
		if (arena_destroy(arena) != 0) {
			return -1;
		}
	}

	arena.type = ARENA_ALLOC;
	arena.size = size;
	arena.store = (uint8_t *)calloc(sizeof(uint8_t), size);
	if (arena.store == NULL) {
		return -1;
	}

	return 0;
}


#if defined(__linux__)
int
mmap_arena(Arena &arena, int fd, size_t size)
{
	if (arena.size > 0) {
		if (arena_destroy(arena) != 0) {
			return -1;
		}
	}

	arena.type = ARENA_MMAP;
	arena.size = size;
	arena.store = (uint8_t *)mmap(NULL, size, PROT_RW, MAP_SHARED, fd, 0);
	if ((void *)arena.store == MAP_FAILED) {
		return -1;
	}
	arena.fd = fd;
	return 0;
}


int
open_arena(Arena &arena, const char *path)
{
	struct stat	st;

	if (arena.size > 0) {
		if (arena_destroy(arena) != 0) {
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

	return mmap_arena(arena, arena.fd, (size_t)st.st_size);
}


int
create_arena(Arena &arena, const char *path, size_t size, mode_t mode)
{
	int	fd = 0;

	if (arena.size > 0) {
		if (arena_destroy(arena) != 0) {
			return -1;
		}
	}

	fd = open(path, O_WRONLY|O_CREAT, mode);
	if (fd == -1) {
		return -1;
	}

	if (ftruncate(fd, size) == -1) {
		return -1;
	}

	close(fd);

	return open_arena(arena, path);
}
#endif


void
arena_clear(Arena &arena)
{
	if (arena.size == 0) {
		return;
	}

	memset(arena.store, 0, arena.size);
}


int
arena_destroy(Arena &arena)
{
	if (arena.type == ARENA_UNINIT) {
		return 0;
	}

	switch (arena.type) {
	case ARENA_STATIC:
		break;
	case ARENA_ALLOC:
		free(arena.store);
		break;
	case ARENA_MMAP:
		if (munmap(arena.store, arena.size) == -1) {
			return -1;
		}

		if (close(arena.fd) == -1) {
			return -1;
		}

		arena.fd = 0;
		break;
	default:
		abort();
		return -1;
	}

	arena.type = ARENA_UNINIT;
	arena.size = 0;
	arena.store = NULL;
	return 0;
}


int
write_arena(Arena &arena, const char *path)
{
	FILE	*arenaFile = NULL;
	int	 retc = -1;

	arenaFile = fopen(path, "w");
	if (arenaFile == NULL) {
		return -1;
	}

	if (fwrite(arena.store, sizeof(*arena.store), arena.size,
		   arenaFile) == arena.size) {
		retc = 0;
	}

	if (fclose(arenaFile) != 0) {
		return -1;
	}

	return retc;
}

