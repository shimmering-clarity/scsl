#ifndef KLIB_DICTIONARY_H
#define KLIB_DICTIONARY_H


#include "Arena.h"
#include "TLV.h"


#define DICTIONARY_TAG_KEY	1
#define DICTIONARY_TAG_VAL	2


/*
 * A Dictionary is a collection of key-value pairs, similar to how
 * a dictionary is a mapping of names to definitions.
 */
class Dictionary {
public:
	Dictionary(Arena &arena) :
	    arena(arena),
	    kTag(DICTIONARY_TAG_KEY),
	    vTag(DICTIONARY_TAG_VAL) {} ;
	Dictionary(Arena &arena, uint8_t kt, uint8_t vt) :
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
