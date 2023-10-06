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


uint8_t	*write_to_memory(Arena &, uint8_t *, Record &);
void	 read_from_memory(Record &, uint8_t *);
void	 set_record(Record &, uint8_t, uint8_t, const char *);
void	 delete_record(Arena &, uint8_t *);

/* 
 * returns a pointer to memory where the record was found,
 * e.g. find_tag(...)[0] is the tag of the found record.
 */
uint8_t *find_tag(Arena &, uint8_t *, Record &);
uint8_t	*find_empty(Arena &, uint8_t *);
uint8_t	*skip_record(Record &, uint8_t *);


} // namespace TLV


#endif
