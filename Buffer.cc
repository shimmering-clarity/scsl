//
// Created by kyle on 2023-10-09.
//

#include <cassert>
#include <cstring>
#include "Buffer.h"

namespace klib {


constexpr size_t	defaultCapacity = 32;
constexpr size_t	maxReasonableLine = 8192;


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

	return x;
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
	size_t	datalen = strnlen(data, maxReasonableLine);

	this->Append((uint8_t *)data, datalen);
}


bool
Buffer::Append(uint8_t *data, size_t datalen)
{
	auto	resized = false;
	auto	newCap = this->mustGrow(datalen);

	if (newCap > 0) {
		this->Resize(newCap);
		resized = true;
	}

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
Buffer::Insert(size_t index, uint8_t *data, size_t datalen)
{
	auto	resized = this->shift(index, datalen);

	memcpy(this->contents + index, data, datalen);
	return resized;
}

bool
Buffer::Insert(size_t index, uint8_t c)
{
	return this->Insert(index, &c, 1);
}


void
Buffer::Resize(size_t newCapacity)
{
	if (newCapacity < this->length) {
		return;
	}

	uint8_t *newContents = new uint8_t[newCapacity];

	if (this->length > 0) {
		memcpy(newContents, this->contents, this->length);
	}

	if (this->contents != nullptr) {
		delete this->contents;
	}

	this->contents = newContents;
	this->capacity = newCapacity;
}

size_t
Buffer::Trim()
{
	size_t	projectedCapacity = nearestPower(this->length);

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
	memset(this->contents, 0, this->length);
	this->length = 0;
}

void
Buffer::Reclaim()
{
	delete this->contents;
	this->length = 0;
	this->capacity = 0;
}

size_t
Buffer::mustGrow(size_t delta)
{
	if ((delta + this->length) < this->capacity) {
		return 0;
	}

	auto newCapacity = delta + this->length;
	return nearestPower(newCapacity);
}


bool
Buffer::shift(size_t offset, size_t delta)
{
	auto	resized = false;
	auto	newCap = this->mustGrow(delta);

	if (newCap > 0) {
		this->Resize(newCap);
		resized = true;
	}

	
	for (size_t i = this->length; i >= offset; i++) {
		this->contents[i+delta] = this->contents[i];
	}

	return resized;
}


} // namespace klib
