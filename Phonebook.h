#ifndef KLIB_PHONEBOOK_H
#define KLIB_PHONEBOOK_H


#include "Arena.h"
#include "TLV.h"


#define PHONEBOOK_KEY_TAG	1
#define PHONEBOOK_VAL_TAG	2



class Phonebook {
public:
	Phonebook(Arena &arena) :
		arena(arena),
		ktag(PHONEBOOK_KEY_TAG),
		vtag(PHONEBOOK_VAL_TAG) {} ;
	Phonebook(Arena &arena, uint8_t kt, uint8_t vt) :
		arena(arena),
		ktag(kt),
		vtag(vt) {};

	bool	lookup(const char *key, uint8_t klen, TLV::Record &res);
	int	set(const char *key, uint8_t klen, char *val, uint8_t vlen);
	bool	has(const char *key, uint8_t klen);

private:
	uint8_t	*seek(const char *key, uint8_t klen);

	Arena	&arena;
	uint8_t	 ktag;
	uint8_t	 vtag;
};


#endif
