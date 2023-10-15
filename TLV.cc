#include <cstring>
#include "TLV.h"

using namespace scsl;


#define REC_SIZE(x)	((std::size_t)x.Len + 2)


namespace scsl {
namespace TLV {


static bool
spaceAvailable(Arena &arena, uint8_t *cursor, uint8_t len)
{
	if (cursor == nullptr) {
		return false;
	}

	return arena.CursorInArena(cursor + len);
}

static inline void
clearUnused(Record &rec)
{
	uint8_t trail = TLV_MAX_LEN - rec.Len;

	memset(rec.Val + rec.Len, 0, trail);
}


uint8_t *
WriteToMemory(Arena &arena, uint8_t *cursor, Record &rec)
{
	// If cursor is nullptr, the user needs us to select an empty
	// slot for the record. If we can't find one, that's an
	// error.
	//
	// If, however, the user gives us a cursor, we'll trust it
	// (though spaceAvailable will sanity check that cursor).
	if (cursor == nullptr) {
		cursor = FindEmpty(arena, cursor);
		if (cursor == nullptr) {
			return nullptr;
		}
	}

	if (!arena.CursorInArena(cursor)) {
		return nullptr;
	}

	if (!spaceAvailable(arena, cursor, rec.Len)) {
		return nullptr;
	}

	memcpy(cursor, &rec, REC_SIZE(rec));
	cursor = SkipRecord(rec, cursor);

	return cursor;
}


void
SetRecord(Record &rec, uint8_t tag, uint8_t len, const char *val)
{
	rec.Tag = tag;
	rec.Len = len;
	memcpy(rec.Val, val, len);
	clearUnused(rec);
}


void
ReadFromMemory(Record &rec, uint8_t *cursor)
{
	rec.Tag = cursor[0];
	rec.Len = cursor[1];
	memcpy(rec.Val, cursor + 2, rec.Len);
	clearUnused(rec);
}


/* 
 * returns a pointer to memory where the record was found,
 * e.g. FindTag(...)[0] is the tag of the found record.
 */
uint8_t *
FindTag(Arena &arena, uint8_t *cursor, Record &rec)
{
	cursor = LocateTag(arena, cursor, rec);
	if (rec.Tag != TAG_EMPTY) {
		cursor = SkipRecord(rec, cursor);
	}

	return cursor;
}


uint8_t *
LocateTag(Arena &arena, uint8_t *cursor, Record &rec)
{
	uint8_t tag, len;

	if (cursor == nullptr) {
		cursor = arena.Start();
	}

	while ((tag = cursor[0]) != rec.Tag) {
		len = cursor[1];
		if (!spaceAvailable(arena, cursor, len)) {
			return nullptr;
		}
		cursor += len;
		cursor += 2;
	}

	if (tag != rec.Tag) {
		return nullptr;
	}

	if (tag != TAG_EMPTY) {
		ReadFromMemory(rec, cursor);
	}
	return cursor;
}


uint8_t *
FindEmpty(Arena &arena, uint8_t *cursor)
{
	Record rec;

	rec.Tag = TAG_EMPTY;
	return FindTag(arena, cursor, rec);
}


uint8_t *
SkipRecord(Record &rec, uint8_t *cursor)
{
	return (uint8_t *) ((uintptr_t) cursor + rec.Len + 2);
}


void
DeleteRecord(Arena &arena, uint8_t *cursor)
{
	if (cursor == nullptr) {
		return;
	}

	if (!arena.CursorInArena(cursor)) {
		return;
	}

	uint8_t len = cursor[1] + 2;
	uint8_t *stop = arena.Start() + arena.Size();

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
} // namespace scsl