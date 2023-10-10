#include <cstring>
#include <cstdlib>
#include "Dictionary.h"

#if defined(DESKTOP_BUILD)
#include <iostream>
#endif


namespace klib {


bool
Dictionary::Lookup(const char *key, uint8_t klen, TLV::Record &res)
{
	res.Tag = this->kTag;
	uint8_t	*cursor = TLV::FindTag(this->arena, nullptr, res);

	while (cursor != nullptr) {
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
	uint8_t		*cursor = nullptr;

	SetRecord(rec, this->kTag, klen, key);
	cursor = this->seek(key, klen);
	if (cursor != nullptr) {
		TLV::DeleteRecord(this->arena, cursor);
		TLV::DeleteRecord(this->arena, cursor);
	}

	if (!spaceAvailable(klen, vlen)) {
		return -1;
	}

	cursor = TLV::WriteToMemory(this->arena, nullptr, rec);
	if (cursor == nullptr) {
		return -1;
	}

	SetRecord(rec, this->vTag, vlen, val);
	if (TLV::WriteToMemory(this->arena, nullptr, rec) == nullptr) {
		return -1;
	}

	return 0;
}


/// seek searches the Dictionary for the key.
uint8_t	*
Dictionary::seek(const char *key, uint8_t klen)
{
	TLV::Record	 rec;

	rec.Tag = this->kTag;
	uint8_t	*cursor = TLV::LocateTag(this->arena, nullptr, rec);

	while (cursor != nullptr) {
		if ((klen == rec.Len) && (this->kTag == rec.Tag)) {
			if (memcmp(rec.Val, key, klen) == 0) {
				return cursor;
			}
		}
		cursor = TLV::SkipRecord(rec, cursor);
		cursor = TLV::LocateTag(this->arena, cursor, rec);
	}

	return nullptr;
}


bool
Dictionary::Contains(const char *key, uint8_t klen)
{
	return this->seek(key, klen) != nullptr;
}


bool
Dictionary::Delete(const char *key, uint8_t klen)
{
	auto	*cursor = this->seek(key, klen);

	if (cursor == nullptr) {
		return false;
	}

	TLV::DeleteRecord(this->arena, cursor);
	TLV::DeleteRecord(this->arena, cursor);
	return true;
}


bool
Dictionary::spaceAvailable(uint8_t klen, uint8_t vlen)
{
	size_t		 required = 0;
	uintptr_t	 remaining = 0;
	uint8_t		*cursor = nullptr;

	cursor = TLV::FindEmpty(this->arena, nullptr);
	if (cursor == nullptr) {
		return false;
	}

	required += klen + 2;
	required += vlen + 2;

	remaining = (uintptr_t)cursor - (uintptr_t)arena.NewCursor();
	remaining = arena.Size() - remaining;
	return ((size_t)remaining >= required);
}


std::ostream &
operator<<(std::ostream &os, const Dictionary &dictionary)
{
#if defined(DESKTOP_BUILD)
	uint8_t 	*cursor = (dictionary.arena).NewCursor();
	TLV::Record	 rec;

	TLV::ReadFromMemory(rec, cursor);
	if (rec.Tag == TLV::TAG_EMPTY) {
		os << "\t(NONE)" << std::endl;
		return os;
	}

	while ((cursor != nullptr) && (rec.Tag != TLV::TAG_EMPTY)) {
		os << "\t" << rec.Val << "->";
		cursor = TLV::SkipRecord(rec, cursor);
		TLV::ReadFromMemory(rec, cursor);
		os << rec.Val << std::endl;
		cursor = TLV::SkipRecord(rec, cursor);
		TLV::ReadFromMemory(rec, cursor);
	}
#endif

	return os;
}


int
Dictionary::DumpToFile(const char *path)
{
	return this->arena.Write(path);
}


} // namespace klib