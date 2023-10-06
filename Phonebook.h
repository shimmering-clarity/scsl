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
	    kTag(PHONEBOOK_KEY_TAG),
	    vTag(PHONEBOOK_VAL_TAG) {} ;
	Phonebook(Arena &arena, uint8_t kt, uint8_t vt) :
	    arena(arena),
	    kTag(kt),
	    vTag(vt) {};

	bool	Lookup(const char *key, uint8_t klen, TLV::Record &res);
	int	Set(const char *key, uint8_t klen, const char *val,
		       uint8_t vlen);
	bool	Has(const char *key, uint8_t klen);
	void	DumpKVPairs();
	void	DumpToFile(const char *path);
private:
	uint8_t	*seek(const char *key, uint8_t klen);
	bool	 spaceAvailable(uint8_t klen, uint8_t vlen);

	Arena	&arena;
	uint8_t	 kTag;
	uint8_t	 vTag;
};


#endif
