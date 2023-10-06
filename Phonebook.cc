#include <cstring>
#include <cstdlib>
#include "Phonebook.h"

bool
Phonebook::lookup(const char *key, uint8_t klen, TLV::Record &res)
{
	res.Tag = this->ktag;
	uint8_t	*cursor = TLV::find_tag(this->arena, NULL, res);

	while (cursor != NULL) {
		if ((klen == res.Len) &&
		    (memcmp(res.Val, key, klen) == 0)) {
			TLV::read_from_memory(res, cursor);
			if (res.Tag != this->vtag) {
				abort();
			}
			return true;
		}
		cursor = TLV::find_tag(this->arena, cursor, res);
	}

	return false;

}


int
Phonebook::set(const char *key, uint8_t klen, const char *val, uint8_t vlen)
{
	TLV::Record	 rec;
	uint8_t		*cursor = NULL;

	set_record(rec, this->ktag, klen, key);
	cursor = this->seek(key, klen);
	if (cursor != NULL) {
		TLV::delete_record(this->arena, cursor);
		TLV::delete_record(this->arena, cursor);
	}

	if (!space_available(klen, vlen)) {
		return -1;
	}

	cursor = TLV::write_to_memory(this->arena, NULL, rec);
	if (cursor == NULL) {
		return -1;
	}

	set_record(rec, this->vtag, vlen, val);
	if (TLV::write_to_memory(this->arena, NULL, rec) == NULL) {
		return -1;
	}

	return 0;
}



uint8_t	*
Phonebook::seek(const char *key, uint8_t klen)
{
	TLV::Record	 rec;

	rec.Tag = this->ktag;
	uint8_t	*cursor = TLV::find_tag(this->arena, NULL, rec);

	while (cursor != NULL) {
		if ((klen == rec.Len) &&
		    (memcmp(rec.Val, key, klen) == 0)) {
			return cursor;
		}
		cursor = TLV::skip_record(rec, cursor);
	}

	return NULL;
}


bool
Phonebook::has(const char *key, uint8_t klen)
{
	return this->seek(key, klen) != NULL;
}


bool
Phonebook::space_available(uint8_t kl, uint8_t vl)
{
	size_t		 required = 0;
	uintptr_t	 remaining = 0;
	uint8_t		*cursor = NULL;


	cursor = TLV::find_empty(this->arena, NULL);
	if (cursor == NULL) {
		return false;
	}

	required += kl + 2;
	required += vl + 2;

	remaining = (uintptr_t)cursor - (uintptr_t)arena.store;
	remaining = arena.size - remaining;
	return ((size_t)remaining >= required);
}
