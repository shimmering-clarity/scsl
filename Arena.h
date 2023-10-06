#ifndef KIMODEM_ARENA_H
#define KIMODEM_ARENA_H


#include <sys/stat.h>
#include <cstddef>
#include <cstdint>


typedef struct {
	uint8_t	*store;
	size_t	 size;
	int	 fd;
	uint8_t	 type;
} Arena;


int 	new_arena(Arena &, uint8_t *, size_t);
int 	alloc_new_arena(Arena &, size_t);
#if defined(__linux__)
int 	mmap_arena(Arena &, int); /* arena will own fd */
int	create_arena(Arena &arena, const char *path, size_t size, mode_t mode);
int 	open_arena(Arena &, const char *, size_t);
#endif

void	arena_clear(Arena &);
int	arena_destroy(Arena &); /* dispose of any memory used by arena */

/* DANGER: if arena is file backed (mmap or open), DO NOT WRITE TO THE
 * BACKING FILE! */
int	write_arena(const char *);


#endif
