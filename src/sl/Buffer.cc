///
/// \file Buffer.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-09
/// \brief Buffer implements basic line buffers.
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

#include <cassert>
#include <cstring>
#include <iomanip>
#include <ios>
#include <iostream>

#include <scsl/Buffer.h>


namespace scsl {


/// The defaultCapacity for a new Buffer is a reasonably arbitrary starting
/// point.
constexpr size_t defaultCapacity = 32;
/// maxReasonableLine is the longest a reasonable line could be. It assumes
/// something like a long, unprettified JSON strong or the like.
constexpr size_t maxReasonableLine = 8192;


static size_t
nearestPower(size_t x)
{
	if (x == 0) {
		return 0;
	}

	x--;

	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x |= x >> 32;

	return x + 1;
}


Buffer::Buffer()
    : contents(nullptr), length(0), capacity(0), autoTrim(true)
{
	this->Resize(defaultCapacity);
}


Buffer::Buffer(size_t initialCapacity)
    : contents(nullptr), length(0), capacity(0), autoTrim(true)
{
	this->Resize(initialCapacity);
}


Buffer::Buffer(const char *data)
    : contents(nullptr), length(0), capacity(0), autoTrim(true)
{
	size_t datalen = strnlen(data, maxReasonableLine);

	this->Append((uint8_t *) data, datalen);
}


Buffer::Buffer(const std::string& s)
    : contents(nullptr), length(0), capacity(0), autoTrim(true)
{
	this->Append(s);
}


Buffer::~Buffer()
{
	this->Reclaim();
}

uint8_t *
Buffer::Contents() const
{
	return this->contents;
}


std::string
Buffer::ToString() const
{
	return std::string((const char *)(this->contents));
}


size_t
Buffer::Length() const
{
	return this->length;
}


size_t
Buffer::Capacity() const
{
	return this->capacity;
}


bool
Buffer::Append(const char *s)
{
	size_t slen = strnlen(s, maxReasonableLine);

	return this->Append((uint8_t *) s, slen);
}


bool
Buffer::Append(const std::string &s)
{
	return this->Append((const uint8_t *) s.c_str(), s.size());
}


bool
Buffer::Append(const uint8_t *data, const size_t datalen)
{
	if (datalen == 0) {
		return false;
	}

	auto resized = false;
	auto newCap = this->mustGrow(datalen);

	if (newCap > 0) {
		this->Resize(newCap);
		resized = true;
	} else if (this->contents == nullptr) {
		this->Resize(this->capacity);
		resized = true;
	}

	// If newCap is > 0, memory will be allocated for this->
	// contents, and if contents was null, then Resize should force
	// it to be allocated. Still, a little defensive coding never
	// hurt.
	assert(this->contents != nullptr);
	if (this->contents == nullptr) {
		return false;
	}

	memcpy(this->contents + this->length, data, datalen);
	this->length += datalen;
	return resized;
}


bool
Buffer::Append(const uint8_t c)
{
	return this->Append(&c, 1);
}


bool
Buffer::Insert(const size_t index, const char *s)
{
	size_t slen = strnlen(s, maxReasonableLine);

	return this->Insert(index, (uint8_t *) (s), slen);
}


bool
Buffer::Insert(const size_t index, const std::string &s)
{
	return this->Insert(index, (const uint8_t *) s.c_str(), s.size());
}


bool
Buffer::Insert(const size_t index, const uint8_t *data, const size_t datalen)
{
	auto resized = this->shiftRight(index, datalen);

	memcpy(this->contents + index, data, datalen);
	this->length += datalen;
	return resized;
}


bool
Buffer::Insert(const size_t index, const uint8_t c)
{
	return this->Insert(index, &c, 1);
}


bool
Buffer::Remove(const size_t index, const size_t count)
{
	auto resized = this->shiftLeft(index, count);

	this->length -= count;
	return resized;
}


bool
Buffer::Remove(size_t index)
{
	return this->Remove(index, 1);
}


void
Buffer::Resize(size_t newCapacity)
{
	if (newCapacity < this->length) {
		newCapacity = nearestPower(this->length + newCapacity);
	}

	auto newContents = new uint8_t[newCapacity];

	memset(newContents, 0, newCapacity);

	// Defensive coding check.
	if ((this->length > 0) && (this->contents == nullptr)) {
		abort();
	}

	if (this->length > 0 && this->contents != nullptr) {
		memcpy(newContents, this->contents, this->length);
	}

	if (this->length > 0) {
		delete[] this->contents;
		this->contents = nullptr;
	}
	this->contents = newContents;
	this->capacity = newCapacity;
}

size_t
Buffer::Trim()
{
	size_t projectedCapacity = nearestPower(this->length);

	assert(projectedCapacity >= length);

	if (projectedCapacity < this->capacity) {
		this->Resize(projectedCapacity);
		return this->Capacity();
	}

	return 0;
}


void
Buffer::DisableAutoTrim()
{
	this->autoTrim = false;
}


void
Buffer::EnableAutoTrim()
{
	this->autoTrim = true;
}


bool
Buffer::AutoTrimIsEnabled()
{
	return this->autoTrim;
}


void
Buffer::Clear()
{
	if (this->length == 0) {
		return;
	}

	memset(this->contents, 0, this->length);
	this->length = 0;
}

void
Buffer::Reclaim()
{
	this->Clear();

	if (this->contents == nullptr) {
		assert(this->length == 0);
		assert(this->capacity == 0);
		return;
	}

	delete[] this->contents;
	this->contents = nullptr;
	this->capacity = 0;
}

size_t
Buffer::mustGrow(size_t delta) const
{
	if ((delta + this->length) < this->capacity) {
		return 0;
	}

	auto newCapacity = delta + this->length;
	return nearestPower(newCapacity);
}


void
Buffer::HexDump(std::ostream &os)
{
#ifndef NDEBUG
	size_t index = 0;

	os << std::hex;
	os << std::setfill('0');

	for (index = 0; index < this->length; index++) {
		bool eol = (index % 16) == 0;
		if (eol && (index > 0)) {
			os << std::endl;
		}

		if (eol) {
			os << std::setw(8);
			os << index << "  ";
			os << std::setw(2);
		}

		os << (unsigned short) this->contents[index];

		if ((index % 15) != 0 || (index == 0)) {
			os << " ";
		}
	}

	if ((index % 16) != 0) {
		os << std::endl;
	}

	os << std::setw(0) << std::dec;
#else
  (void)os;
#endif
}

bool
Buffer::shiftRight(size_t offset, size_t delta)
{
	auto resized = false;
	auto newCap = this->mustGrow(delta);

	if (newCap > 0) {
		this->Resize(newCap);
		resized = true;
	}

	if (this->length < offset) {
		for (size_t i = this->length; i < offset; i++) {
			this->contents[i] = ' ';
		}
	}


	memmove(this->contents + (offset + delta), this->contents + offset,
		this->length);
	return resized;
}


bool
Buffer::shiftLeft(size_t offset, size_t delta)
{
	if (delta == 0) {
		return false;
	}

	if ((offset+delta) > this->length) {
		abort();
	}

	for (auto i = offset; i <= (this->length-delta); i++) {
		this->contents[i] = this->contents[i+delta];
	}

	for (auto i = this->length-delta; i < this->length; i++) {
		this->contents[i] = 0;
	}

	if (this->AutoTrimIsEnabled()) {
		return this->Trim() != 0;
	}
	return false;
}


uint8_t &
Buffer::operator[](size_t index)
{
	if (index > this->length) {
#if defined(SCSL_DESKTOP_BUILD) and !defined(SCSL_NOEXCEPT)
		throw std::range_error("array index out of bounds");
#else
		abort();
#endif
	}
	return this->contents[index];
}


bool
operator==(const Buffer &lhs, const Buffer &rhs)
{
	if (lhs.length != rhs.length) {
		return false;
	}

	return memcmp(lhs.contents, rhs.contents, rhs.length) == 0;
}


std::ostream &
operator<<(std::ostream &os, const Buffer &buf)
{
//	std::string s((const char *)buf.Contents(), buf.Length());
	os << const_cast<uint8_t *>(buf.Contents());
	return os;
}


} // namespace scsl
