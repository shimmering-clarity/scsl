#include <iostream>

#include "Buffer.h"


int
main()
{
	klib::Buffer	buffer("hlo, world");

	std::cout << buffer.Contents() << std::endl;

	buffer.Insert(1, (uint8_t *)"el", 2);
	buffer.Append('!');

	std::cout << buffer.Contents() << std::endl;

	buffer.Remove(buffer.Length()-1);
	buffer.Remove(0, 5);
	buffer.Insert(0, 'g');
	buffer.Insert(1, (uint8_t *)"oodbye", 6);

	buffer.Reclaim();
	buffer.Append("and now for something completely different...");

	std::cout << buffer.Contents() << std::endl;

	return 0;
}
