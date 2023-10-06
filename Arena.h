#ifndef KIMODEM_ARENA_H
#define KIMODEM_ARENA_H


#include <sys/stat.h>
#include <cstddef>
#include <cstdint>


typedef struct {
	uint8_t	*Store;
	size_t	 Size;
	int	 fd;
	uint8_t	 Type;
} Arena;


/*
 * InitializeArena is intended for use only with systems that
 * do not initialize new variables to zero. It should be called
 * exactly once, at the start of the program. Any other time the
 * arena needs to be reset, it should be called with clear_arena
 * or destroy_arena.
 */
void	InitializeArena(Arena &arena);
int 	NewStaticArena(Arena &, uint8_t *, size_t);
int 	AllocNewArena(Arena &, size_t);
#if defined(__linux__)
int 	MMapArena(Arena &, int); /* arena will own fd */
int	CreateArena(Arena &arena, const char *path, size_t size, mode_t mode);
int 	OpenArena(Arena &, const char *, size_t);
#endif

void	ClearArena(Arena &);
int	DestroyArena(Arena &); /* dispose of any memory used by arena */

/* DANGER: if arena is file backed (mmap or open), DO NOT WRITE TO THE
 * BACKING FILE! */
int	WriteArena(const Arena &arena, const char *path);

void	DisplayArena(const Arena &arena);


#endif
