///
/// \file Buffer.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-09
/// \brief Buffer implements basic line buffers.
///
/// Buffer implements a basic uint8_t line buffer that is intended for use in text
/// editing. It allocates memory in powers of two, and will grow or shrink
/// as needed.
///
/// Copyright 2023 K. Isom <kyle@imap.cc>
///
/// Permission to use, copy, modify, and/or distribute this software for
/// any purpose with or without fee is hereby granted, provided that
/// the above copyright notice and this permission notice appear in all /// copies.
///
/// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
/// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
/// AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
/// DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
/// OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
/// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
/// PERFORMANCE OF THIS SOFTWARE.
///

#ifndef KGE_BUFFER_H
#define KGE_BUFFER_H

#include <cstdint>
#include <iostream>


namespace scsl {

/// Buffer is a basic line buffer.
///
/// The buffer manages its own internal memory, growing and shrinking
/// as needed. Its capacity is separate from its length; the optimal
/// capacity is determined as the nearest power of two that is greater
/// than the length of the buffer. For example, if the buffer has a
/// length of 5 bytes, 8 bytes will be allocated. If the buffer is 9
/// bytes, 16 bytes will be allocated.
///
/// The #Append and #Insert methods will call #Resize as necessary to grow
/// the buffer. Similarly the #Remove methods will call #Trim to reclaim some
/// memory if possible, but only if #AutoTrimIsEnabled (it is by default).
class Buffer {
public:
	/// A Buffer can be constructed empty, with no memory allocated (yet).
	Buffer();

	/// A Buffer can be constructed with an explicit capacity.
	///
	/// \param initialCapacity The initial allocation size for the buffer.
	explicit Buffer(size_t initialCapacity);

	/// A Buffer can be initialized with a starting C-style string.
	explicit Buffer(const char *s);

	/// A Buffer can be initialized with a starting string.
	explicit Buffer(const std::string s);

	~Buffer()
	{ this->Reclaim(); }

	/// Contents returns the Buffer's contents.
	uint8_t *Contents() const
	{ return this->contents; }

	/// Length returns the length of the data currently stored in the
	/// buffer.
	size_t Length() const
	{ return this->length; };

	/// Capacity returns the amount of memory allocated to the Buffer.
	size_t Capacity() const
	{ return this->capacity; }

	/// Append copies in a C-style string to the end of the buffer.
	///
	/// \param s The string to append.
	/// \return True if the Buffer was resized.
	bool Append(const char *s);

	/// Append copies in a string to the end of the buffer.
	///
	/// \param s The string to append.
	/// \return True if the Buffer was resized.
	bool Append(const std::string s);

	/// Append copies in a byte buffer to the end of the buffer.
	///
	/// \param data The byte buffer to insert.
	/// \param datalen The length of the byte buffer.
	/// \return True if the Buffer was resized.
	bool Append(const uint8_t *data, const size_t datalen);

	/// Append copies a single character to the end of the buffer.
	///
	/// \param c The character to append.
	/// \return True if the Buffer was resized.
	bool Append(const uint8_t c);

	/// Insert copies a C-style string into the buffer at index.
	///
	/// \param index The index to insert the string at.
	/// \param s The string to insert.
	/// \return True if the Buffer was resized.
	bool Insert(const size_t index, const char *s);

	/// Insert copies a string into the buffer at index.
	///
	/// \param index The index the string should be inserted at.
	/// \param s The string to insert.
	/// \return True if the Buffer was resized.
	bool Insert(const size_t index, const std::string s);

	/// Insert copies a uint8_t buffer into the buffer at index.
	///
	/// \param index The index to insert the buffer at.
	/// \param data The buffer to insert.
	/// \param datalen The size of the data buffer.
	/// \return True if the Buffer was resized.
	bool
	Insert(const size_t index, const uint8_t *data, const size_t datalen);

	/// Insert copies a character into the buffer at index.
	///
	/// \param index The index to insert the character at.
	/// \param c The character to insert.
	/// \return True if the Buffer was resized.
	bool Insert(const size_t index, const uint8_t c);

	/// Remove removes `count` bytes from the buffer at `index`.
	///
	/// \param index The starting index to remove bytes from.
	/// \param count The number of bytes to remove.
	/// \return True if the Buffer was resized.
	bool Remove(const size_t index, const size_t count);

	/// Remove removes a single byte from the buffer.
	///
	/// \param index The index pointing to the byte to be removed.
	/// \return True if the Buffer was resized.
	bool Remove(size_t index); // remove single char

	/* memory management */

	/// Resize changes the capacity of the buffer to `newCapacity`.
	///
	/// If newCapacity is less than the length of the Buffer, it
	/// will remove enough bytes from the end to make this happen.
	///
	/// \param newCapacity The new capacity for the Buffer.
	void Resize(size_t newCapacity);

	/// Trim will resize the Buffer to an appropriate size based on
	/// its length.
	///
	/// \return The new capacity of the Buffer.
	size_t Trim();

	/// DisableAutoTrim prevents the #Buffer from automatically
	/// trimming memory after a call to #Remove.
	void DisableAutoTrim()
	{ this->autoTrim = false; }

	/// EnableAutoTrim enables automatically trimming memory after
	/// calls to #Remove.
	void EnableAutoTrim()
	{ this->autoTrim = true; }

	/// AutoTrimIsEnabled returns true if autotrim is enabled.
	///
	/// \return #Remove will call Trim.
	bool AutoTrimIsEnabled()
	{ return this->autoTrim; }

	/// Clear removes the data stored in the buffer. It will not
	/// call #Trim; the capacity of the buffer will not be altered.
	void Clear();

	/// Reclaim the memory in the buffer: the buffer will call #Clear,
	/// followed by deleting any allocated memory.
	void Reclaim();

	/// HexDump dumps the data in the buffer to the output stream;
	/// it is intended as a debugging tool.
	///
	/// \param os The output stream to write to.
	void HexDump(std::ostream &os);

	/// This operator allows the data in the buffer to be accessed
	/// as if it were an array. If the index is out of bounds, it
	/// will throw a range_error.
	///
	/// \throws std::range_error.
	///
	/// \param index The index to retrieve.
	/// \return
	uint8_t &operator[](size_t index);

	/// Two buffers are equal if their lengths are the same and
	/// their contents are the same. Equality is irrespective of
	/// their capacities.
	friend bool operator==(const Buffer &lhs, const Buffer &rhs);

private:
	size_t mustGrow(size_t delta) const;

	bool shiftRight(size_t offset, size_t delta);

	bool shiftLeft(size_t offset, size_t delta);

	uint8_t *contents;
	size_t length;
	size_t capacity;
	bool autoTrim;
};

/// The << operator is overloaded to write out the contents of the Buffer.
std::ostream &operator<<(std::ostream &os, const Buffer &buf);

/// Two Buffers are not equal if their lengths differ or if their contents
/// differ.
inline bool operator!=(const Buffer &lhs, const Buffer &rhs) { return !(lhs == rhs); };

} // namespace scsl


#endif // KGE_BUFFER_H
