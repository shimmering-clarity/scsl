//
// Created by kyle on 2023-10-09.
//

#ifndef KGE_BUFFER_H
#define KGE_BUFFER_H

#include <cstdint>


namespace klib {


class Buffer {
public:
	Buffer();
	Buffer(size_t);
	Buffer(const char *);

	uint8_t	*Contents() { return this->contents; }
	size_t	 Size() { return this->length; };
	size_t	 Capacity() { return this->capacity; }

	bool	 Append(uint8_t *data, size_t datalen);
	bool	 Append(uint8_t c);
	bool	 Insert(size_t index, uint8_t *data, size_t datalen);
	bool	 Insert(size_t index, uint8_t c);

	// bool	 Remove(size_t index, size_t length);

	/* memory management */
	void	 Resize(size_t newCapacity);
	size_t	 Trim();
	void	 Clear();
	void	 Reclaim();

private:
	size_t	 mustGrow(size_t delta);
	bool	 shift(size_t offset, size_t delta);

	uint8_t	*contents;
	size_t	 length;
	size_t	 capacity;
};

} // namespace klib

#endif //KGE_BUFFER_H
