///
/// \file scsl.h
/// \author kyle
/// \date 2023-10-06
///


#ifndef SCSL_DICTIONARY_H
#define SCSL_DICTIONARY_H


#include "Arena.h"
#include "TLV.h"


static constexpr uint8_t	DICTIONARY_TAG_KEY = 1;
static constexpr uint8_t	DICTIONARY_TAG_VAL = 2;


namespace scsl {


/*
 * A Dictionary is a collection of key-value pairs, similar to how
 * a dictionary is a mapping of names to definitions.
 */
/// Dictionary implements a key-value store on top of Arena and TLV::Record.
///
/// phonebook of SSIDs and WPA keys on a microcontroller. This phonebook had to
/// be stored in persistent NVRAM storage, preëmpting the use of std::map or
/// similar. The hardware in use was also not conducive to more expensive
/// options. It was originally named Phonebook until it was adapted to a more
/// general-purpose data structure.
///
/// Keys and vales are stored as sequential pairs of TLV records; they are
/// expected to contain string values but this isn't necessarily the case. The
/// tag values default to a tag of DICTIONARY_TAG_KEY, and values to a tag of
/// DICTIONARY_TAG_VAL.
class Dictionary {
public:
	/// A Dictionary can be initialized with just a backing Arena.
	///
	/// \param arena The backing arena for the Dictionary.
	Dictionary(Arena &arena) :
	    arena(arena),
	    kTag(DICTIONARY_TAG_KEY),
	    vTag(DICTIONARY_TAG_VAL)
	{};

	/// A Dictionary can also be configured with custom key and value types.
	///
	/// \param arena The backing arena for the Dictionary.
	/// \param kt The value to use for key tags.
	/// \param vt The value to use for val tags.
	Dictionary(Arena &arena, uint8_t kt, uint8_t vt) :
	    arena(arena),
	    kTag(kt),
	    vTag(vt)
	{};

	/// Lookup checks to see if the Dictionary has a value under key.
	///
	/// \param key The key to search for.
	/// \param klen The length of the key.
	/// \param res The TLV::Record to store the value in;
	/// \return True if the key was found, false otherwise.
	bool Lookup(const char *key, uint8_t klen, TLV::Record &res);

	/// Set adds a pairing for key → value in the Dictionary.
	///
	/// If the key is already present in the dictionary, both the
	/// key and value are deleted, and a new pair is insert.
	///
	/// \warning If the key is present, but there isn't enough space to
	/// store the new Val, the Dictionary will not contain either key
	/// or value.
	///
	///
	/// \param key The key to associate.
	/// \param klen The length of the key.
	/// \param val The value to associate.
	/// \param vlen The length of the value.
	/// \return Returns 0 on success and -1 on failure.
	int Set(const char *key, uint8_t klen, const char *val,
		uint8_t vlen);

	/// Contains checks the dictionary to see if it contains a given key.
	///
	/// \param key The key to look up.
	/// \param klen The length of the key.
	/// \return True if the key is in the Dictionary, otherwise false.
	bool Contains(const char *key, uint8_t klen);

	/// Delete removes the key from the Dictionary.
	///
	/// \param key The key to look up.
	/// \param klen The length of the key.
	/// \return True if the key was removed, otherwise false.
	bool Delete(const char *key, uint8_t klen);


	/// DumpToFile is a wrapper aorund a call to Arena::Write on the
	/// underlying Arena.
	///
	/// \param path The path to the dumped file.
	/// \return 0 on success, -1 on failure.
	int DumpToFile(const char *path);

	/// operator<< writes the key pairs phonto the output stream.
	///
	/// \param os The output stream to write to.
	/// \param dictionary The dictionary to write out.
	/// \return The output stream is returned.
	friend std::ostream &operator<<(std::ostream &os,
	    const Dictionary &dictionary);
private:
	uint8_t *seek(const char *key, uint8_t klen);

	bool spaceAvailable(uint8_t klen, uint8_t vlen);

	Arena &arena;
	uint8_t kTag;
	uint8_t vTag;
};


} // namespace scsl

#endif
