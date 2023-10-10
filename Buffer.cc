//
// Created by kyle on 2023-10-09.
//

#include <cassert>
#include <cstring>
#include <ios>
#include <iostream>
#include <iomanip>

#include "Buffer.h"

namespace klib {


constexpr size_t defaultCapacity = 32;
constexpr size_t maxReasonableLine = 8192;


static size_t
nearestPower(size_t x)
{
	if (x == 0) {
		return 0;
	}

	std::cout << "x -> ";

	x--;

	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x |= x >> 32;

	std::cout << x + 1 << std::endl;

	return x + 1;
}


Buffer::Buffer()
    : contents(nullptr), length(0), capacity(0)
{
	this->Resize(defaultCapacity);
}


Buffer::Buffer(size_t initialCapacity)
    : contents(nullptr), length(0), capacity(0)
{
	this->Resize(initialCapacity);
}


Buffer::Buffer(const char *data)
    : contents(nullptr), length(0), capacity(0)
{
	size_t datalen = strnlen(data, maxReasonableLine);

	this->Append((uint8_t *) data, datalen);
}


bool
Buffer::Append(const char *s)
{
	size_t slen = strnlen(s, maxReasonableLine);

	return this->Append((uint8_t *) s, slen);
}

bool
Buffer::Append(uint8_t *data, size_t datalen)
{
	auto resized = false;
	auto newCap = this->mustGrow(datalen);

	if (newCap > 0) {
		this->Resize(newCap);
		resized = true;
	}

	assert(this->contents != nullptr);
	memcpy(this->contents + this->length, data, datalen);
	this->length += datalen;
	return resized;
}

bool
Buffer::Append(uint8_t c)
{
	return this->Append(&c, 1);
}


bool
Buffer::Insert(size_t index, const char *s)
{
	size_t slen = strnlen(s, maxReasonableLine);

	return this->Insert(index, (uint8_t *) (s), slen);
}


bool
Buffer::Insert(size_t index, uint8_t *data, size_t datalen)
{
	auto resized = this->shiftRight(index, datalen);

	memcpy(this->contents + index, data, datalen);
	this->length += datalen;
	return resized;
}

bool
Buffer::Insert(size_t index, uint8_t c)
{
	return this->Insert(index, &c, 1);
}


bool
Buffer::Remove(size_t index, size_t count)
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
	if (this->length > 0) {
		memcpy(newContents, this->contents, this->length);
	}

	delete this->contents;
	this->contents = nullptr;
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
	std::cout << "clear" << std::endl;
	this->Clear();

	std::cout << "nullptr check" << std::endl;
	if (this->contents == nullptr) {
		std::cout << "assert checks" << std::endl;
		assert(this->length == 0);
		assert(this->capacity == 0);
		return;
	}

	std::cout << "delete " << this->Capacity() << "B" << std::endl;
	this->HexDump(std::cout);
	delete this->contents;
	std::cout << "reset contents" << std::endl;
	this->contents = nullptr;
	std::cout << "reset capacity" << std::endl;
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
	size_t		index = 0;

	os << std::hex;
	os << std::setfill('0');

	for (index = 0; index < this->length; index++) {
		bool	eol = (index % 16) == 0;
		if (eol && (index > 0)) {
			os << std::endl;
		}

		if (eol) {
			os << std::setw(8);
			os << index << "  ";
			os << std::setw(2);
		}

		os << (unsigned short)this->contents[index];

		if ((index % 15) != 0 || (index == 0)) {
			os << " ";
		}
	}

	if ((index % 16) != 0) {
		os << std::endl;
	}

	os << std::setw(0) << std::dec;
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

	if (this->length == 0) return 0;

	memmove(this->contents + (offset + delta), this->contents + offset, this->length);
	return resized;
}


bool
Buffer::shiftLeft(size_t offset, size_t delta)
{
//	for (size_t i = offset; i < this->length; i++) {
//		this->contents[i] = this->contents[i+delta];
//	}

	memmove(this->contents + offset, this->contents + (offset + delta), this->length);

	return this->Trim() != 0;
}


uint8_t &
Buffer::operator[](size_t index)
{
	return this->contents[index];
}


} // namespace klib
