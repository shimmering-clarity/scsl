#include <cstring>
#include "TLV.h"


#define REC_SIZE(x)	((std::size_t)x.Len + 2)


namespace TLV {


static bool
space_available(Arena &arena, uint8_t *cursor, uint8_t len)
{
	uintptr_t	remaining = 0;

	if (cursor == NULL) {
		return false;
	}

	remaining = (uintptr_t)cursor - (uintptr_t)arena.store;
	remaining = arena.size - remaining;
	return ((size_t)remaining >= ((size_t)len+2));
}


uint8_t *
write_to_memory(Arena &arena, uint8_t *cursor, Record &rec)
{
	// If cursor is NULL, the user needs us to select an empty
	// slot for the record. If we can't find one, that's an
	// error.
	//
	// If, however, the user gives us a cursor, we'll trust it
	// (though space_available will sanity check that cursor).
	if (cursor == NULL) {
		cursor = find_empty(arena, cursor);
		if (cursor == NULL) {
			return NULL;
		}
	}

	if (!space_available(arena, cursor, rec.Len)) {
		return NULL;
	}

	memcpy(cursor, &rec, REC_SIZE(rec));
	cursor = skip_record(rec, cursor);

	return cursor;
}


void	
set_record(Record &rec, uint8_t tag, uint8_t len, const char *val)
{
	uint8_t	trail = TLV_MAX_LEN-len;

	rec.Tag = tag;
	rec.Len = len;
	memcpy(rec.Val, val, len);
	memset(rec.Val+len, 0, trail);
}


void
read_from_memory(Record &rec, uint8_t *cursor)
{
	rec.Tag = cursor[0];
	rec.Len = cursor[1];
	memcpy(rec.Val, cursor+2, rec.Len);
}


/* 
 * returns a pointer to memory where the record was found,
 * e.g. find_tag(...)[0] is the tag of the found record.
 */
uint8_t *
find_tag(Arena &arena, uint8_t *cursor, Record &rec)
{
	uint8_t	tag, len;

	if (cursor == NULL) {
		cursor = arena.store;
	}

	while ((tag = cursor[0]) != rec.Tag) {
		len = cursor[1];
		if (!space_available(arena, cursor, len)) {
			return NULL;
		}
		cursor += len;
		cursor += 2;
	}

	if (tag != rec.Tag) {
		return NULL;
	}

	if (tag != TAG_EMPTY) {
		read_from_memory(rec, cursor);
		cursor = skip_record(rec, cursor);
	}
	return cursor;
}


uint8_t *
find_empty(Arena &arena, uint8_t *cursor) {
	Record	rec;
	
	rec.Tag = TAG_EMPTY;
	return find_tag(arena, cursor, rec);
}



uint8_t	*
skip_record(Record &rec, uint8_t *cursor)
{
	return (uint8_t *)((uintptr_t)cursor + rec.Len + 2);
}


void
delete_record(Arena &arena, uint8_t *cursor)
{
	//
	if (cursor == NULL) {
		return;
	}

	uint8_t	 	 len = cursor[1] + 2;
	uint8_t		*stop = arena.store + arena.size;

	stop -= len;

	while (cursor != stop) {
		cursor[0] = cursor[len];
		cursor++;
	}

	stop += len;
	while (cursor != stop) {
		cursor[0] = 0;
		cursor++;
	}
}


} // namespace TLV
