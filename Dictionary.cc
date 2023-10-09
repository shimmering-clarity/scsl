#include <cstring>
#include <cstdlib>
#include "Dictionary.h"

#if defined(DESKTOP_BUILD)
#include <iostream>
#endif

bool
Dictionary::Lookup(const char *key, uint8_t klen, TLV::Record &res)
{
	res.Tag = this->kTag;
	uint8_t	*cursor = TLV::FindTag(this->arena, NULL, res);

	while (cursor != NULL) {
		if ((klen == res.Len) &&
		    (memcmp(res.Val, key, klen) == 0)) {
			TLV::ReadFromMemory(res, cursor);
			if (res.Tag != this->vTag) {
				abort();
			}
			return true;
		}
		cursor = TLV::FindTag(this->arena, cursor, res);
	}

	return false;

}


int
Dictionary::Set(const char *key, uint8_t klen, const char *val, uint8_t vlen)
{
	TLV::Record	 rec;
	uint8_t		*cursor = NULL;

	SetRecord(rec, this->kTag, klen, key);
	cursor = this->seek(key, klen);
	if (cursor != NULL) {
		TLV::DeleteRecord(this->arena, cursor);
		TLV::DeleteRecord(this->arena, cursor);
	}

	if (!spaceAvailable(klen, vlen)) {
		return -1;
	}

	cursor = TLV::WriteToMemory(this->arena, NULL, rec);
	if (cursor == NULL) {
		return -1;
	}

	SetRecord(rec, this->vTag, vlen, val);
	if (TLV::WriteToMemory(this->arena, NULL, rec) == NULL) {
		return -1;
	}

	return 0;
}



uint8_t	*
Dictionary::seek(const char *key, uint8_t klen)
{
	TLV::Record	 rec;

	rec.Tag = this->kTag;
	uint8_t	*cursor = TLV::LocateTag(this->arena, NULL, rec);

	while (cursor != NULL) {
		if ((klen == rec.Len) && (this->kTag == rec.Tag)) {
			if (memcmp(rec.Val, key, klen) == 0) {
				return cursor;
			}
		}
		cursor = TLV::SkipRecord(rec, cursor);
		cursor = TLV::LocateTag(this->arena, cursor, rec);
	}

	return NULL;
}


bool
Dictionary::Has(const char *key, uint8_t klen)
{
	return this->seek(key, klen) != NULL;
}


bool
Dictionary::spaceAvailable(uint8_t klen, uint8_t vlen)
{
	size_t		 required = 0;
	uintptr_t	 remaining = 0;
	uint8_t		*cursor = NULL;

	cursor = TLV::FindEmpty(this->arena, NULL);
	if (cursor == NULL) {
		return false;
	}

	required += klen + 2;
	required += vlen + 2;

	remaining = (uintptr_t)cursor - (uintptr_t)arena.NewCursor();
	remaining = arena.Size() - remaining;
	return ((size_t)remaining >= required);
}


#if defined(DESKTOP_BUILD)
void
Dictionary::DumpKVPairs()
{
	uint8_t 	*cursor = (this->arena).NewCursor();
	TLV::Record	 rec;

	TLV::ReadFromMemory(rec, cursor);
	std::cout << "Dictionary KV pairs" << std::endl;
	if (rec.Tag == TAG_EMPTY) {
		std::cout << "\t(NONE)" << std::endl;
		return;
	}

	while ((cursor != NULL) && (rec.Tag != TAG_EMPTY)) {
		std::cout << "\t" << rec.Val << "->";
		cursor = TLV::SkipRecord(rec, cursor);
		TLV::ReadFromMemory(rec, cursor);
		std::cout << rec.Val << std::endl;
		cursor = TLV::SkipRecord(rec, cursor);
		TLV::ReadFromMemory(rec, cursor);
	}

}
#else
void
Dictionary::DumpKVPairs()
{

}
#endif


void
Dictionary::DumpToFile(const char *path)
{
	this->arena.Write(path);
}
