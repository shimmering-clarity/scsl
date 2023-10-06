#ifndef KIMODEM_TLV_H
#define KIMODEM_TLV_H

#include <cstdint>

#include "Arena.h"


#ifndef TLV_MAX_LEN
#define TLV_MAX_LEN	253
#endif


#define TAG_EMPTY	0


namespace TLV {


struct Record {
	uint8_t		Tag;
	uint8_t		Len;
	char		Val[TLV_MAX_LEN];
};


uint8_t	*WriteToMemory(Arena &, uint8_t *, Record &);
void	 ReadFromMemory(Record &, uint8_t *);
void	 SetRecord(Record &, uint8_t, uint8_t, const char *);
void	 DeleteRecord(Arena &, uint8_t *);

/*
 * returns a pointer to memory where the record was found,
 * e.g. LocateTag(...)[0] is the tag of the found record.
 * FindTag will call LocateTag and then SkipRecord if the
 * tag was found.
 */
uint8_t *FindTag(Arena &, uint8_t *, Record &);
uint8_t *LocateTag(Arena &, uint8_t *, Record &);

uint8_t	*FindEmpty(Arena &, uint8_t *);
uint8_t	*SkipRecord(Record &, uint8_t *);


} // namespace TLV


#endif
