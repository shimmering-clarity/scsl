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
	explicit Buffer(size_t);
	explicit Buffer(const char *);
	~Buffer() { this->Reclaim(); }

	uint8_t	*Contents() { return this->contents; }
	size_t	 Length() const { return this->length; };
	size_t	 Capacity() const { return this->capacity; }

	bool	 Append(const char *s);
	bool	 Append(uint8_t *data, size_t datalen);
	bool	 Append(uint8_t c);

	bool	 Insert(size_t index, const char *s);
	bool	 Insert(size_t index, uint8_t *data, size_t datalen);
	bool	 Insert(size_t index, uint8_t c);

	bool	 Remove(size_t index, size_t count);
	bool	 Remove(size_t index); // remove single char

	/* memory management */
	void	 Resize(size_t newCapacity);
	size_t	 Trim();
	void	 Clear();
	void	 Reclaim();

	uint8_t	&operator[](size_t index);
private:
	size_t	 mustGrow(size_t delta) const;
	bool	 shiftRight(size_t offset, size_t delta);
	bool	 shiftLeft(size_t offset, size_t delta);

	uint8_t	*contents;
	size_t	 length;
	size_t	 capacity;
};



} // namespace klib

#endif //KGE_BUFFER_H
