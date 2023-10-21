///
/// \file include/scsl/TLV.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-06
/// \brief TLV.h implements basic tag-length-value records.
///
/// TLV implements tag-length-value (TLV) records. Each record can have
/// a maximum length of 253 bytes; each TLV record occupies a fixed 255
/// bytes in memory. TLV records don't allocate memory.
///
/// This system uses an Arena as a backing store.
///

#ifndef KIMODEM_TLV_H
#define KIMODEM_TLV_H

#include <array>
#include <cstdint>

#include "Arena.h"


namespace scsl {

/// \brief Tag-length-value record tooling
namespace TLV {


#ifndef TLV_MAX_LEN
static constexpr size_t TLV_MAX_LEN = 253;
#endif

static constexpr uint8_t TAG_EMPTY = 0;


/// \brief Tag-length-value record with single byte tags and lengths.
///
/// TLV records occupy a fixed size in memory, which can be controlled with the
/// TLV_MAX_LEN define. If this isn't defined, it defaults to a size of 253.
/// When writen to an Arena, it occupies Len + 2 bytes. The strings
/// are not null-terminated in the arena.
struct Record {
	/// A Tag is used to identify the type of this record.
	uint8_t Tag;
	/// Len describes the number of bytes stored in #Val.
	uint8_t Len;
	/// Val contains the data in the record.
	uint8_t Val[TLV_MAX_LEN];
};

/// WriteToMemory writes the TLV record into the arena At the location pointed
/// to in the arena.
///
/// \param arena The backing memory store.
/// \param cursor Pointer into the arena's memory.
/// \param rec A TLV record to be serialized.
/// \return A pointer the memory after the record.
uint8_t *WriteToMemory(Arena &arena, uint8_t *cursor, Record &rec);

/// ReadFromMemory reads a record from the memory pointed to by the cursor.
///
/// \param rec The TLV record to be filled in.
/// \param cursor A pointer into an arena's memory store.
void ReadFromMemory(Record &rec, uint8_t *cursor);

/// SetRecord sets a record.
///
/// \param rec The record to be set.
/// \param tag The record's tag.
/// \param length The record's length.
/// \param data The data to fill the record with.
void SetRecord(Record &rec, uint8_t tag, uint8_t length, const char *data);

/// DeleteRecord removes the record from the arena. All records ahead of this
/// record are shifted backwards so that there are no gaps.
void DeleteRecord(Arena &arena, uint8_t *cursor);

/*
* returns a pointer to memory where the record was found,
* e.g. LocateTag(...)[0] is the tag of the found record.
* FindTag will call LocateTag and then SkipRecord if the
* tag was found.
*/
/// FindTag finds the next occurrence of the record's tag.
///
/// The record must have a tag set, which tells FindTag which tag to look for.
/// If found, it fills the record. \see LocateTag.
///
/// \param arena The backing memory for the TLV store.
/// \param cursor A pointer to memory inside the arena; if it's NULL, the
///     search starts At the beginning of the arena.
/// \param rec The record to be filled.
/// \return If the tag is found, a cursor pointing to the next record is
///     returned; otherwise nullptr is returned.
uint8_t *FindTag(Arena &arena, uint8_t *cursor, Record &rec);

/// LocateTag operates similarly to FindTag, but the cursor points to the
/// beginning of the found record.
///
/// \param arena The backing memory for the TLV store.
/// \param cursor A pointer to memory inside the arena; if it's NULL, the
///     search starts At the beginning of the arena.
/// \param rec The record to be filled.
/// \return If the tag is found, a cursor pointing to the record is
///     returned; otherwise nullptr is returned.
uint8_t *LocateTag(Arena &arena, uint8_t *cursor, Record &rec);

/// FindEmpty finds a pointer the next available empty space.
///
/// \return A cursor to the start of empty space in the arena, or nullptr
///     if there is no more empty space available.
///
/// \param arena The backing memory for the TLV store.
/// \param cursor A pointer to memory inside the arena; if it's NULL, the
///     search starts At the beginning of the arena.
/// \return If the arena has space available, a cursor pointing the start
///     of empty space; otherwise, nullptr is returned.
uint8_t *FindEmpty(Arena &arena, uint8_t *cursor);

/// SkipRecord skips the cursor to the next record.
///
/// \param rec The record that should be skipped.
/// \param cursor A pointer to the record in the arena.
/// \return The pointer to the next record in the arena.
uint8_t *SkipRecord(Record &rec, uint8_t *cursor);


} // namespace TLV
} // namespace scsl


#endif
